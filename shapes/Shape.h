#pragma once
#include "../core/Vector3.h"
#include "../core/Material.h"
#include "../core/Ray.h"
#include "../core/Vertex.h"
#include "../core/RayHit.h"

class Shape
{
public:
	
	virtual void Intersect(const Ray& ray, const std::vector<Vertex> &vertices, RayHit * rayHit, bool smoothShading, int texOffset, bool texturing)  = 0 ;
	virtual Vector3 RandomPoint(const std::vector<Vertex> &vertices) = 0;
	virtual real Area(const std::vector<Vertex> & vertices) = 0;
};





