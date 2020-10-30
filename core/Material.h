#pragma once

#include "Vector3.h"

enum MaterialType
{
	None, Mirror, Dielectric, Conductor
};

enum BRDFs
{
	BLINN_PHONG, MODIFIED_BLINN_PHONG, NORMALIZED_MODIFIED_BLINN_PHONG,
	PHONG, MODIFIED_PHONG, NORMALIZED_MODIFIED_PHONG,
	TORRANCE_SPARROW, TORRANCE_SPARROW_DIFFUSE_FRESNEL
};

class Material
{
public:
	int phongExp = 0;		
	Vector3 ambientRef;		
	Vector3 diffuseRef;		
	Vector3 specularRef;	
	Vector3 mirrorRef;		

	Vector3 absorptionCoef;
	real refractionIndex;
	real absorptionIndex;
	MaterialType type;
	BRDFs brdf = BLINN_PHONG;

	real roughness = 0;
public:
	Vector3 BRDF(const Vector3& toEye, const Vector3& normal, const Vector3& toLight) const;
};