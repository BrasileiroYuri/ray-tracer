#include "math.hpp"
#include <cmath>

// Implementação do Produto Escalar (Dot Product)
float dot(const vec3 &v1, const vec3 &v2) {
    return (v1.i_ * v2.i_) + (v1.j_ * v2.j_) + (v1.k_ * v2.k_);
}

float size(const vec3 &v) {
    return std::sqrt(dot(v, v)); 
}

float size(const vec2 &v) { 
    return std::sqrt((v.i_ * v.i_) + (v.j_ * v.j_)); 
}

vec2 normalize(vec2 v) {
    float s = size(v);
    if (s == 0) return v;
    return {v.i_ / s, v.j_ / s};
}

vec3 normalize(vec3 v) {
    float s = size(v);
    if (s == 0) return v;
    return {v.i_ / s, v.j_ / s, v.k_ / s};
}

vec3 cross(const vec3 &v1, const vec3 &v2) {
    return {
        v1.j_ * v2.k_ - v1.k_ * v2.j_,
        v1.k_ * v2.i_ - v1.i_ * v2.k_,
        v1.i_ * v2.j_ - v1.j_ * v2.i_
    };
}