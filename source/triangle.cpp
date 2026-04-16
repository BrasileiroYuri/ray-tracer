#include "triangle.hpp"
#include <cmath>

bool Triangle::intersect(const Ray& r, float& t_hit) const {
    // Algoritmo de Möller-Trumbore para interseção raio-triângulo
    vec3 edge1 = v1_ - v0_;
    vec3 edge2 = v2_ - v0_;
    vec3 h = cross(r.direction(), edge2);
    float a = dot(edge1, h);

    // Se 'a' for próximo de zero, o raio é paralelo ao triângulo
    if (a > -1e-6 && a < 1e-6) return false;

    float f = 1.0f / a;
    vec3 s = r.origin() - v0_;
    float u = f * dot(s, h);

    // Verifica se a coordenada baricêntrica U está dentro dos limites
    if (u < 0.0f || u > 1.0f) return false;

    vec3 q = cross(s, edge1);
    float v = f * dot(r.direction(), q);

    // Verifica se V e a soma U+V estão dentro dos limites do triângulo
    if (v < 0.0f || u + v > 1.0f) return false;

    // Calcula t para encontrar o ponto de interseção na reta do raio
    float t = f * dot(edge2, q);

    if (t > r.t_min() && t < r.t_max()) {
        t_hit = t;
        return true;
    }

    return false;
}