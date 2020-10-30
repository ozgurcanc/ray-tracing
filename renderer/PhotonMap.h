#pragma once    
#include "../core/Vector3.h"

struct Photon
{
	Vector3 position;
	Vector3 power;
	Vector3 direction;
	
	int split_axis;
	Photon *left = nullptr;
	Photon *right = nullptr;
};


struct NearestPhotons
{
	Vector3 position;
	std::vector<real> distances;
	std::vector<Photon*> photons;
	int capacity;
	int found;
	int heap_constructed;

	NearestPhotons(Vector3 pos, real d, int c)
	{
		position = pos;
		distances = { d };
		photons = { nullptr };
		capacity = c;
		found = heap_constructed = 0;
	}

	void Construct_Heap()
	{
		int n = capacity;
		int i;
		for (i = n / 2; i >= 1; i--)
		{
			int parent = i;
			int child = 2 * i;
			real temp_d = distances[i];
			Photon* temp_p = photons[i];
			while (child <= n)
			{
				if (child<n && distances[child + 1] > distances[child]) child++;
				if (temp_d > distances[child]) break;

				distances[parent] = distances[child];
				photons[parent] = photons[child];
				parent = child;
				child += child;
			}

			distances[parent] = temp_d;
			photons[parent] = temp_p;
		}

		distances[0] = distances[1];
		heap_constructed = 1;
	}
};

class PhotonMap
{
public:

	Photon * root;

	PhotonMap(std::vector<Photon*>& photons);

	Photon * Build_PhotonMap(std::vector<Photon*>& photons, int start, int end, int split_axis);

	void Locate_Photons(NearestPhotons * np);
	void Locate_Photons(NearestPhotons * np, Photon * p);
	Vector3 Radiance_Estimate(Vector3& position, Vector3& normal, real distance, int capacity);
};
