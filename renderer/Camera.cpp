#include "Camera.h"
#include <random>

Camera::Camera(const char* imageName)
{
	std::strcpy(this->imageName, imageName);
}

void Camera::Perspective(const real left, const real right, const real bottom,
	const real top, const real distance, const real nx, const real ny)
{
	imgPlane.left = left;
	imgPlane.right = right;
	imgPlane.bottom = bottom;
	imgPlane.top = top;
	imgPlane.distance = distance;
	imgPlane.nx = nx;
	imgPlane.ny = ny;
}


void Camera::Perspective(const real fovy, const real aspectRatio, const real distance, const real nx)
{
	real h = real_tan(fovy / real(2)) * distance;
	real w = h * aspectRatio;

	imgPlane.left = -w;
	imgPlane.right = w;
	imgPlane.bottom = -h;
	imgPlane.top = h;
	imgPlane.distance = distance;
	imgPlane.nx = nx;
	imgPlane.ny = nx / aspectRatio;
}


void Camera::LookAt(const Vector3& p, const Vector3& t, const Vector3& u, bool right_handed)
{
	position = p;
	gaze = (t - p).Unit();
	right = (gaze % u).Unit();
	up = (right % gaze).Unit();

	if (!right_handed) right *= -1;
}

void Camera::DeriveInternals()
{
	imgPlaneTopLeft = position + gaze * imgPlane.distance + right * imgPlane.left + up * imgPlane.top;
	horizantalStep = right * ((imgPlane.right - imgPlane.left) / imgPlane.nx);
	verticalStep = up * ((imgPlane.top - imgPlane.bottom) / imgPlane.ny);

	totalPixel = imgPlane.nx * imgPlane.ny;
	pthread_mutex_init(&mut, NULL);
}

Ray Camera::GetPrimaryRay(int row, int col, real rowOffset, real colOffset) const
{
	Vector3 pointOnPlane = imgPlaneTopLeft + horizantalStep * (row + rowOffset) - verticalStep * (col + colOffset);
	Vector3 direction = (pointOnPlane - position).Unit();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rng(0.0, 1.0);

	Ray primary_ray(position, direction, rng(gen));

	if (apertureSize)
	{
		real t = focusDistance / (direction * gaze);
		Vector3 p = primary_ray.GetPoint(t);

		std::uniform_real_distribution<> rng_aperture(-apertureSize / 2, apertureSize / 2);

		Vector3 s = position + right * rng_aperture(gen) + up * rng_aperture(gen);

		primary_ray.origin = s;
		primary_ray.direction = (p - s).Unit();
	}

	primary_ray.pixelCoord = Vector2((row + rowOffset) / imgPlane.nx, (col + colOffset) / imgPlane.ny);

	return primary_ray;
}

Vector3 Camera::GetResolution()
{
	return Vector3(imgPlane.nx, imgPlane.ny, 0);
}

void Camera::GetSamples(std::vector<Sample> & samples, int row, int col)
{
	sampler->CreateSamples(samples);

	int nSamples = samples.size();

	for (int i = 0; i < nSamples; i++)
	{
		samples[i].ray = GetPrimaryRay(row, col, samples[i].pos.x, samples[i].pos.y);
		samples[i].pos.x -= 0.5;
		samples[i].pos.y -= 0.5;
	}
}

Vector3 Camera::FilteredColor(std::vector<Sample>& samples)
{
	return filter->FilteredColor(samples);
}

void Camera::EnableMSAA(int numSamples)
{
	if (numSamples <= 3) return;

	int sample = (int)real_sqrt(numSamples);
	MSAA = true;
	sampler = new StratifiedSampler(sample, sample);
	filter = new BoxFilter();
}

bool Camera::GetPixel(int &i, int &j)
{
	pthread_mutex_lock(&mut);
	int curr_pixel = currentPixel;
	currentPixel++;
	pthread_mutex_unlock(&mut);

	if (curr_pixel >= totalPixel) return false;

	i = curr_pixel % imgPlane.nx;
	j = curr_pixel / imgPlane.nx;

	return true;
}


void Camera::EnableToneMapping(real keyValue, real burn_percent, real saturation, real gamma)
{
	tmo = new TMO();
	tmo->keyValue = keyValue;
	tmo->burn_percent = burn_percent;
	tmo->saturation = saturation;
	tmo->gamma = gamma;
}