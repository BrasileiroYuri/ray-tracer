#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive.hpp"

class Triangle : public Primitive {
public:
    Triangle(point3 v0, point3 v1, point3 v2) : v0_(v0), v1_(v1), v2_(v2) {}
    virtual ~Triangle() = default;

    bool intersect(const Ray& r, float& t_hit) const override;

private:
    point3 v0_, v1_, v2_;
};

#endif