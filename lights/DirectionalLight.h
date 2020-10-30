#include "Light.h"

class DirectionalLight : public Light
{
protected:
	Vector3 direction;
	Vector3 intensity;

public:
	DirectionalLight(const Vector3 & d, const Vector3& i);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);
};