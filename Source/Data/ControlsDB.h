#pragma once
#include <string_view>
#include "SDL.h"

enum class GameAction {
  MoveUp,
  MoveDown,
  MoveLeft,
  MoveRight,
  Pause,
  Confirm,
  Cancel,
  Last
};

struct KeyMapping {
  GameAction action;
  SDL_Scancode key;
  std::string_view name;
};

namespace ControlsDB {
inline constexpr KeyMapping KEYBOARD_DEFAULTS[] = {
    {GameAction::MoveUp, SDL_SCANCODE_W, "Move Up"},
    {GameAction::MoveDown, SDL_SCANCODE_S, "Move Down"},
    {GameAction::MoveLeft, SDL_SCANCODE_A, "Move Left"},
    {GameAction::MoveRight, SDL_SCANCODE_D, "Move Right"},
    {GameAction::Pause, SDL_SCANCODE_ESCAPE, "Pause"},
    {GameAction::Cancel, SDL_SCANCODE_ESCAPE, "Cancel"},
    {GameAction::Confirm, SDL_SCANCODE_RETURN, "Confirm"}};

inline constexpr SDL_Scancode GetDefaultKey(GameAction action) {
  for (const auto& map : KEYBOARD_DEFAULTS) {
    if (map.action == action)
      return map.key;
  }
  return SDL_SCANCODE_UNKNOWN;
}

inline constexpr const auto& GetAllMappings() {
  return KEYBOARD_DEFAULTS;
}

inline constexpr size_t GetMappingCount() {
  return sizeof(KEYBOARD_DEFAULTS) / sizeof(KeyMapping);
}

}  // namespace ControlsDB