#include "Color.h"

// Default constructor
Color::Color() : r(255), g(255), b(255), a(255) {}

// Parameterized constructor
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a) {}

// Hex constructor (format: 0xRRGGBBAA)
Color::Color(uint32_t hexRGBA)
    : r((hexRGBA >> 24) & 0xFF),
      g((hexRGBA >> 16) & 0xFF),
      b((hexRGBA >> 8) & 0xFF),
      a(hexRGBA & 0xFF) {}

// Convert to Vector3 (normalized 0-1)
Vector3 Color::ToVector3() const {
  return Vector3{static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f,
                 static_cast<float>(b) / 255.0f};
}

// Static: Convert hex to Color
Color Color::HexToColor(uint32_t hex) {
  return Color(hex);
}

// Static: Fix original implementation
Vector3 Color::HexToVec3(uint32_t hex) {
  // Extract RGBA from 0xRRGGBBAA format
  uint8_t red = (hex >> 24) & 0xFF;
  uint8_t green = (hex >> 16) & 0xFF;
  uint8_t blue = (hex >> 8) & 0xFF;
  return Vector3{static_cast<float>(red) / 255.0f,
                 static_cast<float>(green) / 255.0f,
                 static_cast<float>(blue) / 255.0f};
}

// Define static color constants
const Color Color::White(255, 255, 255, 255);
const Color Color::Black(0, 0, 0, 255);
const Color Color::Red(255, 0, 0, 255);
const Color Color::Green(0, 255, 0, 255);
const Color Color::Blue(0, 0, 255, 255);
const Color Color::Yellow(255, 255, 0, 255);
const Color Color::Magenta(255, 0, 255, 255);
const Color Color::Cyan(0, 255, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);