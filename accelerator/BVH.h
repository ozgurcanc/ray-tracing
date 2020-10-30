#pragma once
#include "BVHCore.h"

struct BucketInfo {
	int count = 0;
	BoundingBox bounds;
};


struct BVHNode
{
public:
	void InitLeaf(const BoundingBox &b,int start,int end);
	void InitInternal(int axis, BVHNode *c0, BVHNode *c1);
	void Intersect(const Ray& ray, const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit, bool smoothShading, int texOffset, bool texturing);

public:
	BoundingBox bounds;
	BVHNode * children[2];
	int splitAxis;
	int start, end;
};


class BVH
{
public:
	BVH();
	BVH(std::vector<PrimitiveInfo>& primitiveInfo,int start,int end);

public:
	BVHNode * root;
	int maxPrimsInNode = 1;
	void Intersect(const Ray& ray,const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit,bool smoothShading, int texOffset, bool texturing);

private:
	BVHNode * BuildBVH(std::vector<PrimitiveInfo>& primitiveInfo, int start, int end);

public:
	int leafNodeCounter;
	int primCounter;
};

struct ObjectInfo
{
public:
	ObjectInfo(BVH * object, Matrix4& transform, int matIndex, Vector3 motion, bool smoothShading, int texIndex[2], int texOffset = 0);

public:
	void Intersect(const Ray& ray, const std::vector<PrimitiveInfo>& primitiveInfo, const std::vector<Vertex>& vertices, RayHit *rayhit);

public:
	BVH *object;

public:
	Matrix4 inverseTransformation;
	Matrix4 transformation;
	Vector3 motion;
	bool smoothShading = false;

	int matIndex;
	int texIndex[2];

	int textureOffset = 0;
	int object_id;
	Vector3 radiance;
};





