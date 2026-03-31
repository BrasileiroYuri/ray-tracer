#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "camera.hpp"
#include "math.hpp"
#include "ray.hpp"

class Orthographic : public Camera {
  Ray generateRay(int x, int y) override {
    float u = getL() + (getR() - getL()) * (x + 0.5) / film_.width();
    float v = getB() + (getT() - getB()) * (y + 0.5) / film_.height();
    return {getOrigin() + getU() * u + getV() * v, getW()};
  }
};

#endif // !ORTHOGRAPHIC_CAMERA_HPP
