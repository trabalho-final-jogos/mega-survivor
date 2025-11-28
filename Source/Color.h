#pragma once
#include <cstdint>
#include "Math.h"

class Color {
 public:
  // Constructors
  Color();
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
  explicit Color(uint32_t hexRGBA);  // New: direct hex construction

  // Conversions
  Vector3 ToVector3() const;
  Vector4 ToVector4() const;
  static Vector4 HexToVec4(uint32_t hex);
  static Color HexToColor(uint32_t hex);
  static Vector3 HexToVec3(uint32_t hex);  // Fixed: now includes alpha

  // Member variables
  uint8_t r, g, b, a;

  // Base color constants (remain static)
  static const Color White;
  static const Color Black;
  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;  // Added common colors
  static const Color Magenta;
  static const Color Cyan;
  static const Color Transparent;
};