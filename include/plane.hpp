#ifndef PLANE_HPP
#define PLANE_HPP

#include "primitive.hpp"

class Plane : public Primitive {
public:
    // P = Ponto inicial, U e V = vetores das arestas
    Plane(point3 p, vec3 u, vec3 v) : p_(p), u_(u), v_(v) {
        // A normal é o produto vetorial das arestas
        vec3 n = cross(u_, v_);
        normal_ = normalize(n);
        // Valor 'd' da equação do plano Ax + By + Cz = d
        d_ = dot(normal_, p_);
        // Vetor auxiliar para otimizar a interseção (proporcional à área)
        w_ = n / dot(n, n);
    }

    bool intersect(const Ray& r, float& t_hit) const override;

private:
    point3 p_;
    vec3 u_, v_, w_, normal_;
    float d_;
};

#endif