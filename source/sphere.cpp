#include "sphere.hpp"
#include <cmath>

// agora recebe a struct Surfel por referência
bool Sphere::intersect(const Ray &r, Surfel &s) const {
  vec3 d_hat = normalize(r.direction_);
  vec3 oc = r.origin_ - center_;

  float parallel_len = dot(oc, d_hat);
  vec3 oc_perp = oc - parallel_len * d_hat;

  float delta = (radius_ * radius_) - dot(oc_perp, oc_perp);
  if (delta < 0)
    return false;
  float sqrt_delta = std::sqrt(delta);

  float t_values[2] = {-parallel_len - sqrt_delta, -parallel_len + sqrt_delta};

  for (float t : t_values) {
    if (t < r.min_t_ || t > r.max_t_)
      continue;

    point3 p = r(t);
    point3 p_local = p - center_;

    if (p_local.k_ < z_min_ || p_local.k_ > z_max_)
      continue;

    float phi = std::atan2(p_local.j_, p_local.i_);
    if (phi < 0)
      phi += 2.0f * (float)M_PI;

    if (phi > phi_max_)
      continue;

    s.t_hit = t; // preenche o membro da struct em vez de variável local
    s.p = p;     // Armazena o ponto exato da interseção para cálculos de luz
    s.n = p_local * (1.0f / radius_); // Calcula e armazena a normal (unitário)

    return true;
  }

  return false;
}
