#include "sphere.hpp"
#include <cmath>

bool Sphere::intersect(const Ray& r, float& t_hit) const {
    vec3 oc = r.origin() - center_;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(oc, r.direction());
    float c = dot(oc, oc) - radius_ * radius_;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    // Encontrar a raiz mais próxima dentro do intervalo do raio
    float sqrtd = std::sqrt(discriminant);
    float root = (-b - sqrtd) / (2.0f * a);
    
    // Verifica se a raiz está no intervalo [t_min, t_max] do raio
    if (root < r.t_min() || root > r.t_max()) {
        root = (-b + sqrtd) / (2.0f * a);
        if (root < r.t_min() || root > r.t_max())
            return false;
    }

    t_hit = root;
    return true;
}