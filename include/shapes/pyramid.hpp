#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "math.hpp"
#include "shape.hpp"
#include <algorithm>
#include <limits>

class Pyramid : public Shape {
public:
    Pyramid(point3 center, float width, float height) 
        : center_{center}, width_{width}, height_{height} {}

    bool intersect(const Ray &r, Surfel &s) const override {
        float t_near = std::numeric_limits<float>::max();
        vec3 hit_normal;
        bool hit = false;

        float w = width_ / 2.0f;
        
        // Vértices calculados a partir do center
        vec3 top = center_ + vec3(0, height_, 0);
        vec3 v0  = center_ + vec3(-w, 0, -w); // Inferior Esquerdo
        vec3 v1  = center_ + vec3(w, 0, -w);  // Inferior Direito
        vec3 v2  = center_ + vec3(w, 0, w);   // Superior Direito
        vec3 v3  = center_ + vec3(-w, 0, w);  // Superior Esquerdo

        struct Face { vec3 a, b, c; };
        
        Face faces[] = {
            {v0, top, v1}, // Face Frontal
            {v1, top, v2}, // Face Direita
            {v2, top, v3}, // Face Traseira
            {v3, top, v0}, // Face Esquerda
            {v0, v1, v2},  // Base (Triângulo 1)
            {v0, v2, v3}   // Base (Triângulo 2)
        };

        for (const auto &f : faces) {
            float t;
            vec3 n;
            if (intersectTriangle(r, f.a, f.b, f.c, t, n)) {
                // Checa o intervalo de visibilidade do raio
                if (t < t_near && t > 0.001f && t < r.max_t_) {
                    t_near = t;
                    hit_normal = n;
                    hit = true;
                }
            }
        }

        if (hit) {
            s.t_hit = t_near;
            s.p = r.origin_ + r.direction_ * s.t_hit;
            s.n = hit_normal; 
            return true;
        }
        return false;
    }

private:
    point3 center_;
    float width_, height_;

    // Algoritmo de Möller-Trumbore 
    bool intersectTriangle(const Ray &r, const vec3 &v0, const vec3 &v1, const vec3 &v2, 
                           float &t, vec3 &normal) const {
        vec3 v0v1 = v1 - v0;
        vec3 v0v2 = v2 - v0;
        
        vec3 pvec = cross(r.direction_, v0v2);
        float det = dot(v0v1, pvec);

        // Se o determinante é próximo de zero, o raio é paralelo ao triângulo
        if (std::abs(det) < 1e-8) return false;
        float invDet = 1.0f / det;

        vec3 tvec = r.origin_ - v0;
        float u = dot(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f) return false;

        vec3 qvec = cross(tvec, v0v1);
        float v = dot(r.direction_, qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f) return false;

        t = dot(v0v2, qvec) * invDet;
        
        normal = normalize(cross(v0v1, v0v2));
        
        return t > 0;
    }
};

#endif // !PYRAMID_HPP

 