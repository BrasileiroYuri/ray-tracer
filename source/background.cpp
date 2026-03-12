#include <array>
#include <cstdint>

struct RGBColor {
  uint8_t R, G, B;
};

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners{colors} {}

  RGBColor lerp(const RGBColor &A, const RGBColor &B, float t) const {
    auto red = (int)((1 - t) * A.R + t * B.R);
    auto green = (int)((1 - t) * A.G + t * B.G);
    auto blue = (int)((1 - t) * A.B + t * B.B);

    return {(uint8_t)red, (uint8_t)green, (uint8_t)blue};
  }

  RGBColor sample_uv(float u, float v) {
    RGBColor t1 = lerp(corners[bl], corners[br], u);
    RGBColor t2 = lerp(corners[tl], corners[tr], u);
    return lerp(t1, t2, v);
  }

private:
  /*
   * C(1)---(2)D
   * |         |
   * |         |
   * A(0)---(3)B
   */
  const std::array<RGBColor, 4> &corners;
  enum corners_e { bl = 0, tl, tr, br };
};
