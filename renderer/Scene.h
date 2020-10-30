#pragma once
#include "../thirdparty/tinyxml2.h"
#include "Camera.h"
#include "../lights/Light.h"
#include "../lights/AreaLight.h"
#include "../lights/DirectionalLight.h"
#include "../lights/EnvironmentLight.h"
#include "../lights/LightMesh.h"
#include "../lights/LightSphere.h"
#include "../lights/PointLight.h"
#include "../lights/SpotLight.h"
#include "../core/Material.h"
#include "../core/Ray.h"
#include "../shapes/Shape.h"
#include "../shapes/Triangle.h"
#include "../shapes/Sphere.h"
#include "Image.h"
#include "../accelerator/BVH.h"
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <thread>
#include "../thirdparty/happly.h"
#include "../textures/Texture.h"
#include "../textures/ImageTexture.h"
#include "../textures/PerlinTexture.h"
#include "PhotonMap.h"

using namespace tinyxml2;

class Scene
{
public:
	int maxRecursionDepth;				
	real intTestEps;					
	real shadowRayEps;					
	Vector3 backgroundColor;			
	Vector3 ambientLight;				
	EnvironmentLight * enviromentLight = nullptr;

	std::vector<Camera *> cameras;		
	std::vector<Light *> lights;		
	std::vector<Material *> materials;	
	std::vector<Texture * > textures;   
	std::vector<Vertex> vertices;
	std::vector<PrimitiveInfo> primitives;
	std::vector<ObjectInfo *> objects;
	Texture * backgroundTexture = nullptr;
	Integrator integrator;

public:
	void CreateScene(const char *xmlPath);
	void RenderScene();

	void PrintSceneElementCount();
private:
	Vector3 CalculatePixelColor(Ray ray, int currentRecursionDepth);
	void RenderHelper(int min, int numCore, Image * image, int nx, int ny, Camera *cam);
	Vector3 Dielectric(const Vector3& wo, const Vector3& rayOrigin, const RayHit* rayhit, real etaI, real etaT, const Vector3& absorption, int currRecursionDepth, real time);
	Vector3 Conductor(const RayHit * rayhit, const Material &materail,int curr_recusion_depth, real time);
	Ray RoughReflectionRay(const Vector3 & pos, const Vector3 & dir, real roughness, real time);

private:
	void ParsePLYFile(const char* filename, int matIndex, bool smooth);

private:
	void ComputeNormal(std::vector<Vertex> &vertices, int i1, int i2, int i3);

private:
	void Photon_Tracing(std::vector<Photon*> &photons, const Ray &photonRay, Vector3 power, int flag, int depth, int caustic);
	PhotonMap * global_photon_map = nullptr;
	PhotonMap * caustic_photon_map = nullptr;
	Vector3 Radiance(const Ray& ray, int depth);
public:
	void Build_Photon_Map();
};
