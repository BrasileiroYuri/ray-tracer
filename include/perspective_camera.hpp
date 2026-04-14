#ifndef PERSPECTIVE_CAMERA_HPP
#define PERSPECTIVE_CAMERA_HPP

#include "camera.hpp"

class Perspective : public Camera {
public:
    virtual Ray generateRay(int x, int y) override {
        float width = (float)film_.width();
        float height = (float)film_.height();

        float u_coord = getL() + (getR() - getL()) * (x + 0.5f) / width;
        float v_coord = getB() + (getT() - getB()) * (y + 0.5f) / height;

        // Direção: (u*U + v*V + 1*W) 
        // somamos o W pois ele já é o sentido do olhar.
        vec3 direction = normalize(getU() * u_coord + getV() * v_coord + getW());

        return Ray(getOrigin(), direction);
    }
};

#endif // !PERSPECTIVE_CAMERA_HPP