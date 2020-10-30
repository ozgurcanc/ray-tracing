#include "BVH.h"

void BVHNode::InitLeaf(const BoundingBox &b, int _start, int _end)
{
	bounds = b;
	children[0] = children[1] = nullptr;
	start = _start;
	end = _end;
}

void BVHNode::InitInternal(int axis, BVHNode *c0, BVHNode *c1)
{
	splitAxis = axis;
	children[0] = c0;
	children[1] = c1;
	bounds = Union(c0->bounds, c1->bounds);
}

void BVHNode::Intersect(const Ray& ray, const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit, bool smoothShading, int texOffset, bool texturing)
{
	if (!bounds.Intersect(ray, rayhit)) return;

	if (children[0] != nullptr)
	{
		children[0]->Intersect(ray, primitiveInfo, vertices, rayhit, smoothShading, texOffset, texturing);
		children[1]->Intersect(ray, primitiveInfo, vertices, rayhit, smoothShading, texOffset, texturing);
	}
	else
	{
		for (int i = start; i < end; i++)
		{
			primitiveInfo[i].primitive->Intersect(ray, vertices, rayhit, smoothShading, texOffset, texturing);
		}
	}
}

BVH::BVH()
{

}

BVH::BVH(std::vector<PrimitiveInfo>& primitiveInfo, int start, int end)
{
	this->root = BuildBVH(primitiveInfo, start, end);
}


BVHNode * BVH::BuildBVH(std::vector<PrimitiveInfo>& primitiveInfo, int start, int end)
{
	BVHNode * node = new BVHNode();

	BoundingBox bounds = primitiveInfo[start].bounds;
	for (int i = start + 1; i < end; i++)
	{
		bounds = Union(bounds, primitiveInfo[end].bounds);
	}

	int primNumber = end - start;

	if (primNumber <= maxPrimsInNode)
	{
		node->InitLeaf(bounds, start, end);

		leafNodeCounter++;
		primCounter += primNumber;

		return node;
	}

	BoundingBox centerBounds(primitiveInfo[start].center);
	for (int i = start + 1; i < end; i++)
	{
		centerBounds = Union(centerBounds, primitiveInfo[i].center);
	}

	int splitAxis = centerBounds.MaximumExtent();

	if (centerBounds.pMax[splitAxis] == centerBounds.pMin[splitAxis])
	{
		node->InitLeaf(bounds, start, end);

		leafNodeCounter++;
		primCounter += primNumber;

		return node;
	}

	const int nBuckets = 2;

	BucketInfo buckets[nBuckets];

	for (int i = start; i < end; i++)
	{
		int bIndex = nBuckets * centerBounds.Offset(primitiveInfo[i].center)[splitAxis];
		if (bIndex == nBuckets) bIndex--;

		assert(bIndex >= 0);
		assert(bIndex < nBuckets);

		if (buckets[bIndex].count == 0)
			buckets[bIndex].bounds = primitiveInfo[i].bounds;
		else
			buckets[bIndex].bounds = Union(buckets[bIndex].bounds, primitiveInfo[i].bounds);

		buckets[bIndex].count++;
	}

	real cost[nBuckets - 1];

	for (int i = 0; i < nBuckets - 1; i++)
	{
		BoundingBox b0, b1;
		int count0 = 0, count1 = 0;
		bool setb0 = true;
		bool setb1 = true;
		for (int j = 0; j <= i; j++)
		{
			if (buckets[j].count > 0)
			{
				if (setb0)
				{
					b0 = buckets[j].bounds;
					setb0 = false;
				}
				else b0 = Union(b0, buckets[j].bounds);

				count0 += buckets[j].count;
			}
		}

		for (int j = i + 1; j < nBuckets; j++)
		{
			if (buckets[j].count > 0)
			{
				if (setb1)
				{
					b1 = buckets[j].bounds;
					setb1 = false;
				}
				else b1 = Union(b1, buckets[j].bounds);

				count1 += buckets[j].count;
			}
		}

		cost[i] = 1 + (count0 * b0.SurfaceArea() + count1 * b1.SurfaceArea()) / bounds.SurfaceArea();
	}

	real minCost = cost[0];
	int minIndex = 0;

	for (int i = 1; i < nBuckets - 1; i++)
	{
		if (cost[i] < minCost)
		{
			minCost = cost[i];
			minIndex = i;
		}
	}

	PrimitiveInfo * pMid = std::partition(&primitiveInfo[start], &primitiveInfo[end - 1] + 1, [=](const PrimitiveInfo &pi)
	{
		int b = nBuckets *
			centerBounds.Offset(pi.center)[splitAxis];
		if (b == nBuckets) b = nBuckets - 1;
		assert(b >= 0);
		assert(b < nBuckets);
		return b <= minIndex;
	});

	int mid = pMid - &primitiveInfo[0];

	node->InitInternal(splitAxis, BuildBVH(primitiveInfo, start, mid), BuildBVH(primitiveInfo, mid, end));

	return node;

}


void BVH::Intersect(const Ray& ray, const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit, bool smoothShading, int texOffset, bool texturing)
{
	if (root)
	{
		root->Intersect(ray, primitiveInfo, vertices, rayhit, smoothShading, texOffset, texturing);
	}
}

ObjectInfo::ObjectInfo(BVH * object, Matrix4& transform, int matIndex, Vector3 motion, bool smoothShading, int texIndex[2], int texOffset)
	:object(object), matIndex(matIndex), smoothShading(smoothShading), motion(motion), transformation(transform), inverseTransformation(transform.GetInverse())
{
	this->texIndex[0] = texIndex[0];
	this->texIndex[1] = texIndex[1];
	textureOffset = texOffset;
}

void ObjectInfo::Intersect(const Ray& ray, const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit)
{
	real previous_t = rayhit->t;

	Vector3 localOrigin;
	Vector3 localDirection;

	if (motion == Vector3())
	{
		localOrigin = inverseTransformation.TransformPoint(ray.origin);
		localDirection = inverseTransformation.TransformDirection(ray.direction);
	}
	else
	{
		Vector3 pos = motion * ray.time;
		Matrix4 m = inverseTransformation * Matrix4::Translate(-pos.x, -pos.y, -pos.z);

		localOrigin = m.TransformPoint(ray.origin);
		localDirection = m.TransformDirection(ray.direction);
	}

	Ray localRay(localOrigin, localDirection, ray.time);

	object->Intersect(localRay, primitiveInfo, vertices, rayhit, smoothShading, textureOffset, texIndex[0] != -1 || texIndex[1] != -1);

	if (previous_t != rayhit->t)
	{
		rayhit->pos = ray.GetPoint(rayhit->t);
		rayhit->toEye = ray.direction * -1;
		rayhit->normal = inverseTransformation.TransformNormal(rayhit->normal).Unit();
		rayhit->matIndex = matIndex;
		rayhit->texIndex[0] = texIndex[0];
		rayhit->texIndex[1] = texIndex[1];
		rayhit->tangent = transformation.TransformDirection(rayhit->tangent);
		rayhit->object_id = object_id;
	}
}


