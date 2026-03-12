#include <array>
#include <cstdint>

struct RGBColor {
  uint8_t R, G, B;
};

class BackGroundColor {
public:
  BackGroundColor(const std::array<RGBColor, 4> &colors) : corners{colors} {}

  RGBColor lerp(const RGBColor &A, const RGBColor &B, float t) const {
    int red = (1 - t) * A.R + t * B.R;
    int green = (1 - t) * A.G + t * B.G;
    int blue = (1 - t) * A.B + t * B.B;

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

/*
#define HEIGHT 200
#define WIDTH 400
#define MAX_SCALE 255

int main(int argc, char *argv[]) {
  RGBColor C{0, 255, 51}, D{255, 255, 51}, B{255, 0, 51}, A = {0, 0, 51};

  std::array<RGBColor, 4> colors{A, C, D, B};
  BackGroundColor b(colors);

  std::printf("P3\n%d %d\n%d\n", WIDTH, HEIGHT, MAX_SCALE);

  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      RGBColor t3 =
          b.sample_uv((float)j / WIDTH - 1, 1.0 - (float)i / HEIGHT - 1);
      std::printf("%d %d %d\n", t3.R, t3.G, t3.B);
    }
  }
  return 0;
}
*/
