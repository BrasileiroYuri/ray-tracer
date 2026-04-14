#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

class Perspective : public Camera {
public:
    virtual Ray generateRay(int x, int y) override {
        float width = (float)film_.width();
        float height = (float)film_.height();

        float y_inverted = (height - 1.0f) - static_cast<float>(y); // Inverte o eixo y, agora tá certo

        float u_coord = getL() + (getR() - getL()) * (x + 0.5f) / width;
        float v_coord = getB() + (getT() - getB()) * (y_inverted + 0.5f) / height;

        vec3 direction = normalize(getU() * u_coord + getV() * v_coord + getW());

        return Ray(getOrigin(), direction);
    }
};

#endif // !PERSPECTIVE_CAMERA_HPP