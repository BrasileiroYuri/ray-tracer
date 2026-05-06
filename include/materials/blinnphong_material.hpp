#ifndef BLINNPHONG_MATERIAL_HPP
#define BLINNPHONG_MATERIAL_HPP

#include "material.hpp"

class BlinnPhongMaterial : public Material {
public:
    RGBColor ka, kd, ks;
    float glossiness;

    BlinnPhongMaterial(const RGBColor& a, const RGBColor& d, const RGBColor& s, float g) 
        : ka(a), kd(d), ks(s), glossiness(g) {}
};

#endif