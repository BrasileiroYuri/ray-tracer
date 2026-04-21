#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <array>
#include <sstream>
#include <string>

struct RGBColor {
  unsigned char R, G, B, A = 255;

  RGBColor(const std::array<int, 3> &arr)
      : R{(unsigned char)arr[0]}, G{(unsigned char)arr[1]},
        B{(unsigned char)arr[2]} {}

  RGBColor(unsigned char R, unsigned char G, unsigned char B)
      : R{R}, G{G}, B{B} {}

  RGBColor() = default;

  std::string str() const {
    std::stringstream ss;
    ss << "{" << R << ", " << G << ", " << B << ", " << A << "}";
    return ss.str();
  }
};

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners_{colors} {}

  RGBColor sample(float u, float v) const {
    RGBColor t1 = lerp(corners_[bl], corners_[br], u);
    RGBColor t2 = lerp(corners_[tl], corners_[tr], u);
    return lerp(t1, t2, v);
  }

private:
  /*
   * The corners:
   *   C(1)---(2)D
   *   |         |
   *   |         |
   *   A(0)---(3)B
   */
  std::array<RGBColor, 4> corners_;

  RGBColor lerp(const RGBColor &A, const RGBColor &B, float t) const {

    auto red = (int)((1 - t) * A.R + t * B.R);
    auto green = (int)((1 - t) * A.G + t * B.G);
    auto blue = (int)((1 - t) * A.B + t * B.B);

    return {(unsigned char)red, (unsigned char)green, (unsigned char)blue};
  }

  enum corners_e {
    bl = 0, //!< Bottom left
    tl,     //!< Top left
    tr,     //!< Top right
    br      //!< Bottom right
  };
};

#endif // !BACKGROUND_HPP
