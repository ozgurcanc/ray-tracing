#include "Light.h"

class LightSphere : public Light, public ObjectInfo
{
public:
	LightSphere(BVH * object, Matrix4& transform, int matIndex, Vector3 motion, bool smoothShading, int texIndex[2], int texOffset);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);

public:

};