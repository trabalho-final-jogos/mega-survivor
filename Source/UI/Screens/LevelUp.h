#pragma once
#include <string>
#include <vector>
#include "../../Components/Upgrades/UpgradeComponent.h"
#include "UIScreen.h"
#include "../../Actors/weapons/WeaponType.h"

enum class UpgradeType {
	Stat,
	Weapon
};

// Estrutura que define um item de upgrade
struct RunUpgrade {
	UpgradeType type;
	Stats statType;         // Usado se for Stat
	WeaponType weaponId; // Usado se for Weapon (Ex: "IceGun")
	std::string description;// O texto que aparece no botão
};

class LevelUp : public UIScreen {
 public:
  LevelUp(class Game* game, const std::string& fontName);
  ~LevelUp();

  void HandleKeyPress(int key) override;

 private:
  std::vector<RunUpgrade> mOptions;
  void SelectUpgrade(RunUpgrade upgrade);
	int mSelectedButtonIndex;
};
