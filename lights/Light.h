#pragma once
#include "../core/Material.h"
#include "../accelerator/BVH.h"
#include "../renderer/Image.h"

class Light
{
public:
	virtual Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices) = 0;
};
