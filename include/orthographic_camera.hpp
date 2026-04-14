#ifndef ORTHOGRAPHIC_CAMERA_HPP
#define ORTHOGRAPHIC_CAMERA_HPP

#include "camera.hpp"

class Orthographic : public Camera {
public:
    virtual Ray generateRay(int x, int y) override {
        float width = (float)film_.width();
        float height = (float)film_.height();

        float y_inverted = (height - 1.0f) - static_cast<float>(y); // Inverte o eixo y (e fica certo, finalmente)

        float u_coord = getL() + (getR() - getL()) * (x + 0.5f) / width;
        float v_coord = getB() + (getT() - getB()) * (y_inverted + 0.5f) / height;

        point3 ray_origin = getOrigin() + (getU() * u_coord) + (getV() * v_coord);
        vec3 direction = normalize(getW());

        return Ray(ray_origin, direction);
    }
};

#endif // !ORTHOGRAPHIC_CAMERA_HPP