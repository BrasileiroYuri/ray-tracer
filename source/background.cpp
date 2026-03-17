#include "background.hpp"
#include <istream>

RGBColor BackGroundColor::sample(float u, float v) const {
  RGBColor t1 = lerp(corners_[bl], corners_[br], u);
  RGBColor t2 = lerp(corners_[tl], corners_[tr], u);
  return lerp(t1, t2, v);
}

RGBColor BackGroundColor::lerp(const RGBColor &A, const RGBColor &B,
                               float t) const {

  auto red = (int)((1 - t) * A.R + t * B.R);
  auto green = (int)((1 - t) * A.G + t * B.G);
  auto blue = (int)((1 - t) * A.B + t * B.B);

  return {(unsigned char)red, (unsigned char)green, (unsigned char)blue};
}

std::istream &operator>>(std::istream &ss, RGBColor &r) {
  ss >> r.R >> r.G >> r.B;
  return ss;
}
