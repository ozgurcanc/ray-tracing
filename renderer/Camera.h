#pragma once

#include "../core/Vector3.h"
#include "../core/Ray.h"
#include <cstring>

#include "../samplers/Sampler.h"
#include "../samplers/StratifiedSampler.h"
#include "../filters/BoxFilter.h"
#include "../filters/GaussianFilter.h"
#include "../filters/LanczosFilter.h"
#include "../filters/MitchellFilter.h"
#include "../filters/TriangleFilter.h"
#include "Image.h"

typedef struct ImagePlane
{
	real left;     
	real right;    
	real bottom;   
	real top;      
	real distance; 
	int nx;         
	int ny;         
} ImagePlane;

struct Integrator
{
	enum IntegratorType {DIRECT_LIGHTING, PATH_TRACING, PHOTON_MAPPING};

	IntegratorType type = DIRECT_LIGHTING;

	// params
	bool NextEventEstimation = false;
	bool ImportanceSampling = false;
	bool RussianRoulette = false;
};

class Camera
{
public:
	char imageName[64];

	Camera(const char* imageName);
	void LookAt(const Vector3& pos, const Vector3& target, const Vector3& up, bool right_handed); 
	void Perspective(const real left, const real right, const real bottom, const real top, const real distance, const real nx, const real ny);
	void Perspective(const real fovy, const real aspectRatio, const real near, const real nx);
	void DeriveInternals();
	Ray GetPrimaryRay(int row, int col, real rowOffset = 0.5, real colOffset= 0.5) const;
	Vector3 GetResolution();
	bool GetPixel(int &i, int &j);

private:
	ImagePlane imgPlane;
	
	Vector3 position;
	Vector3 gaze;
	Vector3 up;
	Vector3 right;

	Vector3 imgPlaneTopLeft;
	Vector3 horizantalStep;
	Vector3 verticalStep;

public:
	bool MSAA = false;
	Sampler * sampler = nullptr;
	Filter * filter = nullptr;

public:
	void GetSamples(std::vector<Sample> & samples, int row, int col);
	Vector3 FilteredColor(std::vector<Sample>& samples);
	void EnableMSAA(int numSamples);

public:
	real focusDistance;
	real apertureSize = 0;

protected:
	int currentPixel = 0;
	int totalPixel;
	pthread_mutex_t  mut;

public:
	TMO * tmo = nullptr;
public:
	void EnableToneMapping(real keyValue, real burn_percent, real saturation, real gamma);

public:
	Integrator integrator;
};

