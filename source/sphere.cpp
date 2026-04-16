#include "sphere.hpp"
#include <cmath>
#include <algorithm>

bool Sphere::intersect(const Ray& r, float& t_hit) const {
    vec3 d_hat = normalize(r.direction());
    vec3 oc = r.origin() - center_; 

    float parallel_len = dot(oc, d_hat);
    vec3 oc_perp = oc - parallel_len * d_hat;

    // Cálculo do discriminante estável para esferas distantes
    float delta = (radius_ * radius_) - dot(oc_perp, oc_perp);
    if (delta < 0) return false;
    float sqrt_delta = std::sqrt(delta);
    
    float t_values[2] = {-parallel_len - sqrt_delta, -parallel_len + sqrt_delta};

    for (float t : t_values) {
        if (t < r.t_min() || t > r.t_max()) continue;

        point3 p = r(t);
        point3 p_local = p - center_;

        if (p_local.k_ < z_min_ || p_local.k_ > z_max_) continue;

        // Verificação de corte em Phi (Azimute)
        float phi = std::atan2(p_local.j_, p_local.i_);
        if (phi < 0) phi += 2.0f * (float)M_PI; // Normaliza para [0, 2*PI]
        
        if (phi > phi_max_) continue;

        t_hit = t;
        return true;
    }

    return false;
}