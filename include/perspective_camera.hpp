#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

class Perspective : public Camera {
  virtual Ray generateRay(int x, int y) override {
    return {getOrigin(), getW() + getU() * x + getV() * y};
  }
};

#endif // !PERSPECTIVE_CAMERA_HPP
