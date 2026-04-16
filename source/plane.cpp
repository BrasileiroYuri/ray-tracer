#include "plane.hpp"
#include <cmath>

bool Plane::intersect(const Ray& r, float& t_hit) const {
    float denom = dot(normal_, r.direction());

    // Se o raio for paralelo ao plano, não há hit
    if (std::abs(denom) < 1e-6) return false;

    // Calcula t usando a equação do plano: dot(n, (P_orig + t*D)) = d
    float t = (d_ - dot(normal_, r.origin())) / denom;

    if (t < r.t_min() || t > r.t_max()) return false;

    // Determina se o ponto de impacto P está dentro das arestas U e V
    point3 intersection = r(t);
    vec3 planar_hit_vector = intersection - p_;
    float alpha = dot(w_, cross(planar_hit_vector, v_));
    float beta = dot(w_, cross(u_, planar_hit_vector));

    // Verifica se as coordenadas baricêntricas do plano estão entre 0 e 1
    if (alpha < 0 || alpha > 1 || beta < 0 || beta > 1) {
        return false;
    }

    t_hit = t;
    return true;
}