#include "ColorPalette.h"
#include <algorithm>  // for std::transform

void ColorPalette::AddColor(const std::string& name, const Color& color) {
  m_colors[name] = color;
}

void ColorPalette::AddColor(const std::string& name, uint32_t hexRGBA) {
  m_colors[name] = Color(hexRGBA);
}

Color ColorPalette::GetColor(const std::string& name) const {
  auto it = m_colors.find(name);
  if (it != m_colors.end()) {
    return it->second;
  }

  return Color::Magenta;
}

Vector3 ColorPalette::GetColorAsVec3(const std::string& name) const {
  Color color = GetColor(name);
  return color.ToVector3();
}

Vector4 ColorPalette::GetColorAsVec4(const std::string& name) const {
  Color color = GetColor(name);
  return color.ToVector4();
}

bool ColorPalette::HasColor(const std::string& name) const {
  return m_colors.find(name) != m_colors.end();
}

void ColorPalette::InitializeBasePalette() {
  AddColor("Yellow_bright", 0xffd300ff);
  AddColor("Blue_bright", 0x2c45ffff);
  AddColor("Red_bright", 0xc8002aff);
  AddColor("Blue_dark", 0x030060ff);
  AddColor("Lime_green", 0x32cd32ff);
  AddColor("Orange", 0xffa500ff);
  AddColor("Red_light", 0xff7f7fff);
  AddColor("Purple_light", 0xa8a8ffff);
  AddColor("UI_Background", 0x1A1A1AFF);  // Dark Gray
  AddColor("UI_Text", 0xFFFFFFFF);        // White
  AddColor("Environment_Sky", 0x87CEEBFF);
  AddColor("FX_Glow", 0xFFD70088);       // Gold (semi-transparent)
  AddColor("Debug_Hitbox", 0xFF00FFFF);  // Magenta (solid)
}