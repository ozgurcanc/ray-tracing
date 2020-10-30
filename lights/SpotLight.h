#include "Light.h"

class SpotLight : public Light
{
protected:
	Vector3 position;
	Vector3 direction;
	Vector3 intensity;
	real cos_max;
	real cos_min;
	real spot;

public:
	SpotLight(const Vector3& p, const Vector3& d, const Vector3& i, real cosa, real cosb, real s);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);
};
