#include "Material.h"

Vector3 Material::BRDF(const Vector3& toEye, const Vector3& normal, const Vector3& toLight) const
{
	Vector3 _brdf;
	real cos_theta = normal * toLight;

	if (cos_theta > 0)
	{
		switch (brdf)
		{
		case BLINN_PHONG:
		{
			Vector3 half = (toEye + toLight).Unit();
			real specFactor = real_pow(std::max(real(0), normal * half), phongExp);
			_brdf = diffuseRef * cos_theta + specularRef * specFactor;
		}
		break;

		case MODIFIED_BLINN_PHONG:
		{
			Vector3 half = (toEye + toLight).Unit();
			real specFactor = real_pow(std::max(real(0), normal * half), phongExp);
			_brdf = (diffuseRef + specularRef * specFactor) * cos_theta;
		}
		break;

		case NORMALIZED_MODIFIED_BLINN_PHONG:
		{
			Vector3 half = (toEye + toLight).Unit();
			real specFactor = real_pow(std::max(real(0), normal * half), phongExp);
			_brdf = (diffuseRef * (1 / R_PI) + specularRef * specFactor * ((phongExp + 8) / (8 * R_PI))) * cos_theta;
		}
		break;

		case PHONG:
		{
			Vector3 reflect = (toLight - normal * (normal * toLight) * 2).Unit();
			real specFactor = real_pow(real_abs(toEye * reflect), phongExp);
			_brdf = diffuseRef * cos_theta + specularRef * specFactor;
		}
		break;

		case MODIFIED_PHONG:
		{
			Vector3 reflect = (toLight - normal * (normal * toLight) * 2).Unit();
			real specFactor = real_pow(real_abs(toEye * reflect), phongExp);
			_brdf = (diffuseRef + specularRef * specFactor) * cos_theta;
		}
		break;

		case NORMALIZED_MODIFIED_PHONG:
		{
			Vector3 reflect = (toLight - normal * (normal * toLight) * 2).Unit();
			real specFactor = real_pow(real_abs(toEye * reflect), phongExp);
			_brdf = (diffuseRef * (1 / R_PI) + specularRef * specFactor * ((phongExp + 2) / (2 * R_PI))) * cos_theta;
		}
		break;

		case TORRANCE_SPARROW:
		{
			Vector3 half = (toEye + toLight).Unit();
			real cos_half_n = std::max(real(0), half * normal);
			real cos_half_eye = std::max(real(0), toEye * half);
			real cos_phi = normal * toEye;
			real distribution = real_pow(cos_half_n, phongExp) * (phongExp + 2) / (2 * R_PI);
			//real parallel = (material.refractionIndex - 1) / (material.refractionIndex + 1);
			//parallel *= parallel;
			//real fresnal = parallel + (1 - parallel) * real_pow(1 - cos_half_eye, 5);
			real eta = refractionIndex;
			real eta_k = absorptionIndex;
			real cosI = cos_phi;
			real cos2I = cosI * cosI;

			real two_cos_eta = 2 * eta * cosI;

			real t0 = eta * eta + eta_k * eta_k;
			real t1 = t0 * cos2I;

			real rs = (t0 - two_cos_eta + cos2I) / (t0 + two_cos_eta + cos2I);
			real rp = (t1 - two_cos_eta + 1) / (t1 + two_cos_eta + 1);

			real fresnel = (rs + rp) / 2;
			real g = (cos_half_n / cos_half_eye) * 2;
			g = std::min(real(1), g * std::min(cos_phi, cos_theta));
			_brdf = diffuseRef * (1 / R_PI) * cos_theta + specularRef * (fresnel * g * distribution / (4 * cos_phi));
		}
		break;

		case TORRANCE_SPARROW_DIFFUSE_FRESNEL:
		{
			Vector3 half = (toEye + toLight).Unit();
			real cos_half_n = std::max(real(0), half * normal);
			real cos_half_eye = std::max(real(0), toEye * half);
			real cos_phi = normal * toEye;
			real distribution = real_pow(cos_half_n, phongExp) * (phongExp + 2) / (2 * R_PI);
			//real parallel = (material.refractionIndex - 1) / (material.refractionIndex + 1);
			//parallel *= parallel;
			//real fresnal = parallel + (1 - parallel) * real_pow(1 - cos_half_eye, 5);
			real eta = refractionIndex;
			real eta_k = absorptionIndex;
			real cosI = cos_phi;
			real cos2I = cosI * cosI;

			real two_cos_eta = 2 * eta * cosI;

			real t0 = eta * eta + eta_k * eta_k;
			real t1 = t0 * cos2I;

			real rs = (t0 - two_cos_eta + cos2I) / (t0 + two_cos_eta + cos2I);
			real rp = (t1 - two_cos_eta + 1) / (t1 + two_cos_eta + 1);

			real fresnel = (rs + rp) / 2;
			real g = (cos_half_n / cos_half_eye) * 2;
			g = std::min(real(1), g * std::min(cos_phi, cos_theta));
			_brdf = diffuseRef * (1 / R_PI) * cos_theta * (1 - fresnel) + specularRef * (fresnel * g * distribution / (4 * cos_phi));
		}
		break;


		default:
			break;
		}
	}

	return _brdf;
}