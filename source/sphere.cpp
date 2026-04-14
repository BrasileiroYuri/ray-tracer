#include "sphere.hpp"
#include <cmath>

bool Sphere::intersect(const Ray& r, float& t_hit) const {

    vec3 d_hat = normalize(r.direction());
    vec3 oc = r.origin() - center_; 

    float parallel_len = dot(oc, d_hat);

    vec3 oc_perp = oc - parallel_len * d_hat;

    float delta = (radius_ * radius_) - dot(oc_perp, oc_perp);
    if (delta < 0) return false;
    float sqrt_delta = std::sqrt(delta);
    
    // Testamos a raiz mais próxima
    float t = -parallel_len - sqrt_delta;

    if (t < r.t_min() || t > r.t_max()) {
        // Testamos a raiz mais distante
        t = -parallel_len + sqrt_delta;
        if (t < r.t_min() || t > r.t_max()) {
            return false;
        }
    }

    t_hit = t;
    return true;
}