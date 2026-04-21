#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "camera.hpp"

struct Orthographic : public Camera {
  Orthographic(CameraConfig cc) : Camera(cc) {}

  virtual Ray generateRay(int x, int y) override {
    float width = (float)film_.width();
    float height = (float)film_.height();

    float y_inverted =
        (height - 1.0f) - static_cast<float>(y); // Inverte o eixo y

    float u_coord = l_ + (r_ - l_) * (x + 0.5f) / width;
    float v_coord = b_ + (t_ - b_) * (y_inverted + 0.5f) / height;

    point3 ray_origin = origin_ + (u_ * u_coord) + (v_ * v_coord);
    vec3 direction = normalize(w_);

    return Ray(ray_origin, direction);
  }
};

#endif // !ORTHOGRAPHIC_CAMERA_HPP
