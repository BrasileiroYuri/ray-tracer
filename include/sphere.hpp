#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "primitive.hpp"

class Sphere : public Primitive {
public:
    Sphere(point3 center, float radius) : center_(center), radius_(radius) {}
    virtual ~Sphere() = default;
    // Implementação da interseção matemática
    bool intersect(const Ray& r, float& t_hit) const override;

private:
    point3 center_;
    float radius_;
};

#endif