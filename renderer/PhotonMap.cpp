#include "PhotonMap.h"

PhotonMap::PhotonMap(std::vector<Photon*>& photons)
{
	root = Build_PhotonMap(photons, 0, photons.size() - 1, 0);
}

Photon * PhotonMap::Build_PhotonMap(std::vector<Photon*>& photons, int start, int end, int split_axis)
{

	if (start > end) return nullptr;

	if (start == end)
	{
		Photon *temp = photons[start];
		temp->split_axis = split_axis;
		return temp;
	}

	std::sort(photons.begin() + start, photons.begin() + end, [&split_axis](Photon* a, Photon* b) { return a->position[split_axis] < b->position[split_axis]; });

	int mid = (start + end) / 2;

	Photon * temp = photons[mid];

	temp->split_axis = split_axis;

	split_axis = (split_axis + 1) % 3;
	temp->left = Build_PhotonMap(photons, start, mid - 1, split_axis);
	temp->right = Build_PhotonMap(photons, mid + 1, end, split_axis);

	return temp;
}

void PhotonMap::Locate_Photons(NearestPhotons * np, Photon * p)
{
	if (p == nullptr) return;
	real dist;


	if (p->left || p->right)
	{
		dist = np->position[p->split_axis] - p->position[p->split_axis];

		if (dist > 0.0)
		{
			Locate_Photons(np, p->right);
			if (dist * dist < np->distances[0])
				Locate_Photons(np, p->left);
		}
		else
		{
			Locate_Photons(np, p->left);
			if (dist * dist < np->distances[0])
				Locate_Photons(np, p->right);
		}
	}

	real dist2 = (p->position - np->position).SquareMagnitude();

	if (dist2 < np->distances[0])
	{
		if (np->found < np->capacity)
		{
			np->found++;
			np->distances.push_back(dist2);
			np->photons.push_back(p);
		}
		else
		{
			if (!np->heap_constructed)
				np->Construct_Heap();

			int parent = 1;
			int child = 2;
			while (child <= np->found)
			{
				if (child < np->found && np->distances[child] < np->distances[child + 1])
					child++;
				if (dist2 > np->distances[child])
					break;

				np->distances[parent] = np->distances[child];
				np->photons[parent] = np->photons[child];
				parent = child;
				child *= 2;
			}

			np->distances[parent] = dist2;
			np->photons[parent] = p;

			np->distances[0] = np->distances[1];
		}
	}


}

void PhotonMap::Locate_Photons(NearestPhotons * np)
{
	Locate_Photons(np, root);
}


Vector3 PhotonMap::Radiance_Estimate(Vector3& position, Vector3& normal, real distance, int capacity)
{
	NearestPhotons * np = new NearestPhotons(position, distance, capacity);

	Locate_Photons(np, root);

	Vector3 radiance;
	real d = 0;

	if (np->found < 8)
	{
		delete np;
		return radiance;
	}

	for (int i = 1; i <= np->found; i++)
	{
		if (np->photons[i]->direction * normal > real(0))
		{
			radiance += np->photons[i]->power;
		}
	}

	radiance *= 1.0 / (R_PI * np->distances[0]);

	delete np;
	return radiance;
}