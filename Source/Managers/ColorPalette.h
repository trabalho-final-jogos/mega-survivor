#pragma once
#include <string>
#include <unordered_map>
#include "../Color.h"

class ColorPalette {
 public:
  static ColorPalette& GetInstance() {
    static ColorPalette instance;
    return instance;
  }

  ColorPalette(const ColorPalette&) = delete;
  ColorPalette& operator=(const ColorPalette&) = delete;

  void AddColor(const std::string& name, const Color& color);
  void AddColor(const std::string& name, uint32_t hexRGBA);
  Color GetColor(const std::string& name) const;
  bool HasColor(const std::string& name) const;
  Vector3 GetColorAsVec3(const std::string& name) const;
  Vector4 GetColorAsVec4(const std::string& name) const;

  // Initialize your game's base palette
  void InitializeBasePalette();

 private:
  ColorPalette() = default;
  ~ColorPalette() = default;

  std::unordered_map<std::string, Color> m_colors;
};