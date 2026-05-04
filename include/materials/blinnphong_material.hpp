#ifndef BLINNPHONG_MATERIAL_HPP
#define BLINNPHONG_MATERIAL_HPP

#include "material.hpp"
#include "math.hpp"

class BlinnPhongMaterial : public Material {
	vec3d kd; /* Diffuse coefficient. */
	vec3d ks; /* Specular coefficient. */
	vec3d ka; /* Ambient coefficient. */
	float glossiness;
};

#endif // !BLINNPHONG_MATERIAL_HPP
