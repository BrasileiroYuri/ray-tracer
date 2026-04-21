#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

struct Perspective : public Camera {
  Perspective(CameraConfig cc) : Camera(cc) {}

  virtual Ray generateRay(int x, int y) override {
    float width = (float)film_.width();
    float height = (float)film_.height();

    float y_inverted =
        (height - 1.0f) - static_cast<float>(y); // Inverte o eixo y

    float u_coord = l_ + (r_ - l_) * (x + 0.5f) / width;
    float v_coord = b_ + (t_ - b_) * (y_inverted + 0.5f) / height;

    vec3 direction = normalize(u_ * u_coord + v_ * v_coord + w_);

    return Ray(origin_, direction);
  }
};

#endif // !PERSPECTIVE_CAMERA_HPP
