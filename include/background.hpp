#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <array>

struct RGBColor {
  unsigned char R, G, B, A = 255;

  RGBColor(const std::array<int, 3> &arr)
      : R{(unsigned char)arr[0]}, G{(unsigned char)arr[1]},
        B{(unsigned char)arr[2]} {}

  RGBColor(unsigned char R, unsigned char G, unsigned char B)
      : R{R}, G{G}, B{B} {}

  RGBColor() = default;
};

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners_{colors} {}
  BackGroundColor() {}
  RGBColor sample(float, float) const;

private:
  /*
   * The corners:
   *   C(1)---(2)D
   *   |         |
   *   |         |
   *   A(0)---(3)B
   */
  std::array<RGBColor, 4> corners_;
  RGBColor lerp(const RGBColor &, const RGBColor &, float) const;

  enum corners_e {
    bl = 0, //!< Bottom left
    tl,     //!< Top left
    tr,     //!< Top right
    br      //!< Bottom right
  };
};

#endif // !BACKGROUND_HPP
