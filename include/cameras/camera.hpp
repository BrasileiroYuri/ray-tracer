#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "film.hpp"
#include "math.hpp"
#include "ray.hpp"

class Camera {
public:
    Camera() : l_{0}, r_{0}, b_{0}, t_{0} {}
    virtual ~Camera() = default;

    void getFrame(point3 look_from, point3 look_at, vec3 up) {
        origin_ = look_from;
        vec3 gaze = look_at - look_from;
        
        // w_ aponta para FRENTE (direção do olhar)
        w_ = normalize(gaze); 
        // u_ aponta para a DIREITA (cross do up com o olhar)
        u_ = normalize(cross(up, w_));
        // v_ aponta para CIMA
        v_ = normalize(cross(w_, u_));
    }

    virtual Ray generateRay(int x, int y) = 0;

    Film film_;

    void window(float l, float r, float b, float t) {
        l_ = l; r_ = r; b_ = b; t_ = t;
    }

    vec3 getW() const { return w_; }
    vec3 getU() const { return u_; }
    vec3 getV() const { return v_; }
    vec3 getOrigin() const { return origin_; }
    float getL() const { return l_; }
    float getR() const { return r_; }
    float getB() const { return b_; }
    float getT() const { return t_; }

private:
    vec3 w_, u_, v_;
    float l_, r_, b_, t_;
    vec3 origin_;
};

#endif // !CAMERA_HPP