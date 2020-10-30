#include "Light.h"

class AreaLight : public Light
{
public:
	Vector3 position;
	Vector3 direction;
	Vector3 intensity;
	real size;
	Vector3 u;
	Vector3 v;

public:
	AreaLight(const Vector3& p, const Vector3& d, const Vector3& i, real s);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);
};