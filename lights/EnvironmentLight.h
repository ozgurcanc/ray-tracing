#include "Light.h" 

class EnvironmentLight : public Light
{
protected:
	Vector3 ** hdrMAP;
	int width;
	int height;

public:
	EnvironmentLight(const Image * image);
	Vector3 ComputeLightContribution(const Material &material, const RayHit *rayHit, const real ray_time, const std::vector<ObjectInfo*>& objects, const std::vector<PrimitiveInfo>& primitives, const std::vector<Vertex>& vertices);
	Vector3 EnviromentColor(const Vector3& direction);
};