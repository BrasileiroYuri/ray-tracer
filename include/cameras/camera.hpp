#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "background.hpp"
#include "film.hpp"
#include "lodepng.h"
#include "math.hpp"
#include "ray.hpp"
#include <cmath>
#include <cstddef>
#include <string>

struct CameraConfig {
  std::string type; /* Tipo da câmera */
  float l_, r_, b_, t_;
  point3 look_from, look_at, up;
  std::size_t w_res, h_res;
  float aspec;
  std::size_t fovy;
};

class Camera {
public:
  virtual ~Camera() = default;

  Camera(CameraConfig cc) : film_(cc.w_res, cc.h_res) {
    getFrame(cc.look_from, cc.look_at, cc.up);

    float ratio = cc.aspec ? cc.aspec : (float)cc.w_res / cc.h_res;

    if (cc.r_ != cc.b_) {
      window(cc.l_, cc.r_, cc.b_, cc.t_);
    } else if (cc.fovy) {
      float h = std::tan((cc.fovy * M_PI / 180.0f) / 2.0f);
      window(-ratio * h, ratio * h, -h, h);
      return;
    }

    /*Apenas ratio */
  }

  void add(std::size_t i, std::size_t j, RGBColor color) {
    film_.add(i, j, color);
  }

  virtual Ray generateRay(int x, int y) = 0;

  std::size_t widht() const { return film_.width(); }
  std::size_t height() const { return film_.height(); }

  void write_image(const std::string &filename, bool ppm) const {

    if (!ppm) {
      lodepng::encode(filename, film_.data(), (unsigned)film_.width(),
                      (unsigned)film_.height());
    } else {
      FILE *f = fopen(filename.c_str(), "w");
      if (!f)
        return;
      fprintf(f, "P3\n%d %d\n255\n", (int)film_.width(), (int)film_.height());
      for (size_t i = 0; i < film_.data().size(); i += 4)
        fprintf(f, "%d %d %d\n", film_.data()[i], film_.data()[i + 1],
                film_.data()[i + 2]);
      fclose(f);
    }
  }

protected:
  Film film_;
  vec3 w_, u_, v_;
  float l_, r_, b_, t_;
  vec3 origin_;

private:
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

  void window(float l, float r, float b, float t) {
    l_ = l;
    r_ = r;
    b_ = b;
    t_ = t;
  }
};

#endif // !CAMERA_HPP
