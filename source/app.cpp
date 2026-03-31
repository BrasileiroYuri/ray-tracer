#include "app.hpp"
#include "background.hpp"
#include "camera.hpp"
#include "film.hpp"
#include "lodepng.h"
#include "math.hpp"
#include "orthographic_camera.hpp"
#include "param_set.hpp"
#include "perspective_camera.hpp"
#include <algorithm>
#include <cmath>
#include <string>

// Acessando as opções do main.cpp
namespace Global {
extern std::string outfile;
extern bool quick;
extern int crop[4];
extern bool has_crop;
} // namespace Global

std::string App::filename_ = "";
Camera App::camera_ = Camera();
bool App::ppm_ = true;
BackGroundColor App::background_ = BackGroundColor();

struct CameraConfig {
  point3 look_from, look_at, up;
};

int fovy = 0;
float aspec = 0.f;
bool has_aspect = false;
CameraConfig config_;
ScreenWindow screen_window_ = ScreenWindow();

void App::backGround(const ParamSet &ps) {
  std::array<RGBColor, 4> arr;
  if (ps.retrieve<std::string>("type") == "colors") {
    arr[0] = ps.retrieve<RGBColor>("bl");
    arr[1] = ps.retrieve<RGBColor>("tl");
    arr[2] = ps.retrieve<RGBColor>("tr");
    arr[3] = ps.retrieve<RGBColor>("br");
  } else {
    RGBColor color = ps.retrieve<RGBColor>("color");
    std::fill(arr.begin(), arr.end(), color);
  }
  background_ = BackGroundColor(arr);
}

void App::film(const ParamSet &ps) {
  std::string xml_file = ps.retrieve<std::string>("filename");
  int x = ps.retrieve<int>("w_res");
  int y = ps.retrieve<int>("h_res");

  // Prioridade CLI > XML
  filename_ = Global::outfile.empty() ? xml_file : Global::outfile;
  if (Global::quick) {
    x /= 4;
    y /= 4;
  }

  ppm_ = ps.retrieve<std::string>("img_type") == "ppm";
  camera_.film_ = Film((std::size_t)x, (std::size_t)y);
}

void App::camera(const ParamSet &ps) {

  auto type = ps.retrieve<std::string>("type");
  if (type == "perspective")
    camera_ = Perspective();
  else
    camera_ = Orthographic();

  fovy = ps.retrieve<int>("fovy");
  aspec = ps.retrieve<float>("frame_aspect_ratio");

  if (ps.has_elem("screen_window"))
    screen_window_ = ps.retrieve<ScreenWindow>("screen_window");
}

void App::lookat(const ParamSet &ps) {
  config_.look_from = ps.retrieve<point3>("look_from", {0, 0, 0});
  config_.look_at = ps.retrieve<point3>("look_at", {0, 0, 0});
  config_.up = ps.retrieve<point3>("up", {0, 0, 0});
}

void App::calculateScreenWindow() {
  float aspectratio =
      aspec ? aspec : (float)camera_.film_.width() / camera_.film_.height();

  if (fovy) {
    int h = std::tan(fovy / 2);
    screen_window_.l_ = -aspectratio * h;
    screen_window_.r_ = aspectratio * h;
    screen_window_.b_ = -h;
    screen_window_.l_ = h;
    return;
  }

  // só com aspec
}
void App::render() {
  std::size_t h = camera_.film_.height(), w = camera_.film_.width();
  camera_.getFrame(config_.look_from, config_.look_at, config_.up);
  camera_.window(screen_window_.l_, screen_window_.r_, screen_window_.b_,
                 screen_window_.t_);

  if (!(fovy + aspec)) // Se diferente de 0, algum dos dois foi dado.
    calculateScreenWindow();

  // Configuração do cropwindow
  int x0 = 0, x1 = w, y0 = 0, y1 = h;
  if (Global::has_crop) {
    x0 = std::max(0, Global::crop[0]);
    x1 = std::min((int)w, Global::crop[1]);
    y0 = std::max(0, Global::crop[2]);
    y1 = std::min((int)h, Global::crop[3]);
  }

  for (int i = y0; i < y1; i++) {
    /*float u1 = screen_window_.l_ + (screen_window_.r_ - screen_window_.l_) *
                                       (i + 0.5) / camera_.film_.width();*/
    for (int j = x0; j < x1; j++) {
      /*    float v1 = screen_window_.b_ + (screen_window_.t_ -
       * screen_window_.b_) (j + 0.5) / camera_.film_.height();*/
      float u = float(j) / float(w - 1);
      float v = 1.f - (float(i) / (float)(h - 1));
      camera_.film_.add(j, i, background_.sample(u, v));
    }
  }

  write_image();
}

void App::write_image() {
  if (!ppm_) {
    lodepng::encode(filename_, camera_.film_.data(),
                    (unsigned)camera_.film_.width(),
                    (unsigned)camera_.film_.height());
  } else {
    FILE *f = fopen(filename_.c_str(), "w");
    fprintf(f, "P3\n%d %d\n255\n", (int)camera_.film_.width(),
            (int)camera_.film_.height());
    for (size_t i = 0; i < camera_.film_.data().size(); i += 4)
      fprintf(f, "%d %d %d\n", camera_.film_.data()[i],
              camera_.film_.data()[i + 1], camera_.film_.data()[i + 2]);
    fclose(f);
  }
}
