#include "Light.h"

class PointLight : public Light
{
protected:
	Vector3 position;
	Vector3 intensity;

public:
	PointLight(const Vector3 & p, const Vector3& i);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);
};