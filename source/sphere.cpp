#include "sphere.hpp"
#include <cmath>

bool Sphere::intersect(const Ray &r, Surfel &s) const {
  vec3 oc{r.origin_.i_ - center_.i_, r.origin_.j_ - center_.j_,
          r.origin_.k_ - center_.k_};

  float a = dot(r.direction_, r.direction_); // ≈1 (direction normalised)
  float b_half = dot(oc, r.direction_);
  float c_val = dot(oc, oc) - radius_ * radius_;
  float disc = b_half * b_half - a * c_val;

  if (disc < 0.0f)
    return false;

  float sqrt_d = std::sqrt(disc);
  float inv_a = 1.0f / a;

  auto try_t = [&](float t) -> bool {
    if (t < r.min_t_ || t > r.max_t_)
      return false;
    point3 hp = r(t);
    vec3 lp{hp.i_ - center_.i_, hp.j_ - center_.j_, hp.k_ - center_.k_};

    // z-clipping (in local sphere space)
    if (lp.k_ < z_min_ || lp.k_ > z_max_)
      return false;

    // phi-clipping
    float phi = std::atan2(lp.j_, lp.i_);
    if (phi < 0.0f)
      phi += 2.0f * static_cast<float>(M_PI);
    if (phi > phi_max_)
      return false;

    s.t_hit = t;
    s.p = hp;

    vec3 outward = normalize(lp);

    // Regra da mão esquerda: para consistência com os triângulos do projeto,
    // a normal deve apontar contra o raio incidente.
    bool front = dot(r.direction_, outward) < 0.0f;
    s.geom_n = front ? outward : outward * -1.0f;
    s.n = s.geom_n;

    float theta = std::acos(std::max(-1.0f, std::min(1.0f, lp.k_ / radius_)));
    float dtheta = theta_max_ - theta_min_;
    s.uv = {phi / (2.0f * static_cast<float>(M_PI)),
            dtheta > 1e-6f ? (theta - theta_min_) / dtheta : 0.0f};
    return true;
  };

  float t0 = (-b_half - sqrt_d) * inv_a;
  float t1 = (-b_half + sqrt_d) * inv_a;

  return try_t(t0) || try_t(t1);
}
