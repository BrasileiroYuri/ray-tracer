#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <array>

struct RGBColor {
  unsigned char R, G, B, A = 255;
};

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners_{colors} {}
  RGBColor sample(float, float) const;

private:
  /*
   * The corners:
   *   C(1)---(2)D
   *   |         |
   *   |         |
   *   A(0)---(3)B
   */
  const std::array<RGBColor, 4> &corners_;
  RGBColor lerp(const RGBColor &, const RGBColor &, float) const;
  enum corners_e {
    bl = 0, //!< Bottom left
    tl,     //!< Top left
    tr,     //!< Top right
    br      //!< Bottom right
  };
};

#endif // !BACKGROUND_HPP
