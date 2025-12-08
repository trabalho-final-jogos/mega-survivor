#include "ControlsMenu.h"
#include "../../Data/ControlsDB.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "SDL.h"

ControlsMenu::ControlsMenu(class Game* game,
                           const std::string& fontName,
                           UIScreen* parent)
    : UIScreen(game, fontName), mParent(parent) {
  if (mParent) {
    mParent->SetVisible(false);
  }
  auto* bg =
      AddRect(Vector2(0.0f, 0.0f), Vector2(1024.0f, 768.0f), 1.0f, 0.0f, 50);
  bg->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.6f));

  AddText("Game controls", Vector2(0.0f, 180.0f), 0.5f, 0.0f, 64, 1024, 100);

  UIText* but[ControlsDB::GetMappingCount() - 1]{nullptr};
  Vector2 basePos{0.0f, 100.0f};
  Vector2 offset{0.0f, -50.0f};
  int i{0};

  for (const auto& map : ControlsDB::GetAllMappings()) {
    const char* keyName = SDL_GetScancodeName(map.key);

    std::string fullText = std::string(map.name) + ": " + keyName;

    but[i] =
        AddText(fullText, basePos + (offset * i), 0.25f, 0.0f, 64, 1024, 100);
    i++;
  }
}

ControlsMenu::~ControlsMenu() {
  if (mParent) {
    mParent->SetVisible(true);
  }
}

void ControlsMenu::HandleKeyPress(int key) {
  if (key == SDLK_ESCAPE || key == SDLK_RETURN) {
    Close();
  }
}