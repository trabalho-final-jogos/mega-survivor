#pragma once

#include <string_view>
#include "../Actors/weapons/WeaponType.h"

enum PlayerChar { MEGAMAN, PROTOMAN, BASS, LAST_CHAR };

struct CharInfo {
  PlayerChar playerChar;
  std::string_view charName;
  WeaponType charWeapon;
};

namespace CharacterDB {
inline constexpr CharInfo INFOS[] = {
    {PlayerChar::MEGAMAN, "Megaman", WeaponType::MainGun},
    {PlayerChar::PROTOMAN, "Protoman", WeaponType::IceGun},
    {PlayerChar::BASS, "Bass", WeaponType::LaserGun}};

inline const CharInfo& Get(PlayerChar character) {
  return INFOS[static_cast<int>(character)];
}
}  // namespace CharacterDB