#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <array>
#include <sstream>
#include <string>
#include <algorithm> 

struct RGBColor {
  
  float r = 0, g = 0, b = 0, a = 1.0f;

  RGBColor(const std::array<int, 3> &arr)
      : r{arr[0]/255.0f}, g{arr[1]/255.0f}, b{arr[2]/255.0f} {}

  // contrutor para float
  RGBColor(const std::array<float, 3> &arr)
      : r{arr[0]}, g{arr[1]}, b{arr[2]}, a{1.0f} {}

  // Construtor direto
  RGBColor(float r_, float g_, float b_, float a_ = 1.0f) 
      : r{r_}, g{g_}, b{b_}, a{a_} {}

  RGBColor() = default;

  // Operadores necessários para as fórmulas: L = Ia*ka + Id*kd + Is*ks
  RGBColor operator*(const RGBColor& other) const { return {r * other.r, g * other.g, b * other.b, a * other.a}; }
  RGBColor operator*(float s) const { return {r * s, g * s, b * s, a}; }
  RGBColor operator+(const RGBColor& other) const { return {r + other.r, g + other.g, b + other.b, a}; }
  
  RGBColor& operator+=(const RGBColor& other) { 
    r += other.r; g += other.g; b += other.b; 
    return *this; 
  }

  std::string str() const {
    std::stringstream ss;
    // Converte os valores de 0.0-1.0 para inteiros 0-255 para exibição no log
    ss << "{" << static_cast<int>(r * 255) 
       << ", " << static_cast<int>(g * 255) 
       << ", " << static_cast<int>(b * 255) << "}";
    return ss.str();
  }

  // Método auxiliar para o Integrator obter a cor base em materiais não-Blinn
  RGBColor getColor() const { return *this; }
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
  std::array<RGBColor, 4> corners_;

  RGBColor lerp(const RGBColor &A, const RGBColor &B, float t) const {
    return A * (1.0f - t) + B * t;
  }

  enum corners_e { bl = 0, tl, tr, br };
};

#endif