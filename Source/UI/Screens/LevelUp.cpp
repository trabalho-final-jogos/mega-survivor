#include "LevelUp.h"
#include <algorithm>
#include <random>
#include "../../Actors/Player.h"
#include "../../Game.h"
#include "../../Components/WeaponComponent.h"


static bool IsWeaponAvailable(Player* player, WeaponType weaponId) {
    WeaponComponent* weapon = player->GetWeaponByType(weaponId);

    // Não tem → pode aparecer (unlock)
    if (!weapon)
        return true;

    // Tem mas ainda não maxou → pode aparecer (level up)
    return weapon->GetLevel() < weapon->GetMaxLevel();
}
// Define available run upgrades locally since RunUpgrade.h is missing
// and we are replacing the system anyway.
const std::vector<RunUpgrade> AVAILABLE_RUN_UPGRADES = {

    // -------- STATS --------
    { UpgradeType::Stat, Stats::Speed,       WeaponType::None, "Increase Speed" },
    { UpgradeType::Stat, Stats::Damage,      WeaponType::None, "Increase Damage" },
    { UpgradeType::Stat, Stats::Area,        WeaponType::None, "Increase Area" },
    { UpgradeType::Stat, Stats::Projectiles, WeaponType::None, "Extra Projectile" },
    { UpgradeType::Stat, Stats::Regen,       WeaponType::None, "Health Regen" },
    { UpgradeType::Stat, Stats::Lucky,       WeaponType::None, "Increase Luck" },
    { UpgradeType::Stat, Stats::Health,      WeaponType::None, "Increase Max HP" },

    // -------- WEAPONS --------
    { UpgradeType::Weapon, Stats::None, WeaponType::IceGun,       "Ice Gun" },
    { UpgradeType::Weapon, Stats::None, WeaponType::LaserGun,     "Laser Gun" },
    { UpgradeType::Weapon, Stats::None, WeaponType::BoomerangGun, "Boomerang" },
    { UpgradeType::Weapon, Stats::None, WeaponType::SawGun,       "Saw Blade" },
    { UpgradeType::Weapon, Stats::None, WeaponType::MainGun,      "Main Gun" },
    { UpgradeType::Weapon, Stats::None, WeaponType::Aura,         "Aura" }
};

// ======================================================
// CONSTRUCTOR / DESTRUCTOR
// ======================================================

LevelUp::LevelUp(Game* game, const std::string& fontName)
    : UIScreen(game, fontName)
{
    mGame->SetPaused(true);
    mGame->GetAudioSystem()->PlaySound("levelUp.wav");

    AddText("LEVEL UP!",
            Vector2(0.0f, 200.0f),
            0.6f, 0.0f, 64, 1024, 150);

    // ---- Randomiza upgrades ----
    Player* player = mGame->GetPlayer();

    std::vector<RunUpgrade> weaponUpgrades;
    std::vector<RunUpgrade> statUpgrades;

    // Separa upgrades válidos
    for (const RunUpgrade& def : AVAILABLE_RUN_UPGRADES) {
        if (def.type == UpgradeType::Weapon) {
            if (player && IsWeaponAvailable(player, def.weaponId)) {
                weaponUpgrades.push_back(def);
            }
        }
        else {
            statUpgrades.push_back(def);
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    std::shuffle(weaponUpgrades.begin(), weaponUpgrades.end(), rng);
    std::shuffle(statUpgrades.begin(), statUpgrades.end(), rng);

    std::vector<RunUpgrade> finalOptions;

    // Sempre tenta pegar 3 weapons
    for (int i = 0; i < 3 && i < static_cast<int>(weaponUpgrades.size()); ++i) {
        finalOptions.push_back(weaponUpgrades[i]);
    }

    // Completa com stats se necessário
    int missing = 3 - static_cast<int>(finalOptions.size());

    for (int i = 0; i < missing && i < static_cast<int>(statUpgrades.size()); ++i) {
        finalOptions.push_back(statUpgrades[i]);
    }

    // Cria botões
    Vector2 pos(0.0f, 80.0f);

    for (const RunUpgrade& upgrade : finalOptions) {

        std::string label = upgrade.description;

        if (upgrade.type == UpgradeType::Weapon && player) {
            WeaponComponent* weapon =
                player->GetWeaponByType(upgrade.weaponId);

            if (weapon) {
                label = "Level Up " + WeaponTypeToString(upgrade.weaponId) +
                        " (Lvl " + std::to_string(weapon->GetLevel() + 1) + ")";
            }
            else {
                label = "Unlock " + WeaponTypeToString(upgrade.weaponId);
            }
        }

        UIButton* btn = AddButton(
            label,
            [this, upgrade]() { SelectUpgrade(upgrade); },
            pos,
            0.5f, 0.0f, 32, 1024, 102
        );

        btn->SetBackgroundColor(
            ColorPalette::GetInstance().GetColorAsVec4("FX_Glow")
        );

        //mButtons.push_back(btn);
        pos.y -= 80.0f;
    }

    mSelectedButtonIndex = 0;
    if (!mButtons.empty()) {
        mButtons[0]->SetHighlighted(true);
        mButtons[0]->SetSelected(true);
    }
}


LevelUp::~LevelUp() {
    SDL_Log("UIScreen::~UIScreen() this=%p", this);
    mGame->SetPaused(false);
}
// ======================================================
// INPUT
// ======================================================

void LevelUp::HandleKeyPress(int key) {
    if (mButtons.empty())
        return;

    int oldIndex = mSelectedButtonIndex;

    switch (key) {
        case SDLK_w:
        case SDLK_UP:
            mSelectedButtonIndex =
                (mSelectedButtonIndex - 1 + mButtons.size()) % mButtons.size();
            break;

        case SDLK_s:
        case SDLK_DOWN:
            mSelectedButtonIndex =
                (mSelectedButtonIndex + 1) % mButtons.size();
            break;

        case SDLK_RETURN:
        case SDLK_KP_ENTER:
            mButtons[mSelectedButtonIndex]->OnClick();
            return;
    }

    if (oldIndex != mSelectedButtonIndex) {
        mButtons[oldIndex]->SetHighlighted(false);
        mButtons[oldIndex]->SetSelected(false);

        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
        mButtons[mSelectedButtonIndex]->SetSelected(true);
    }
}

// ======================================================
// APLICA UPGRADE
// ======================================================

void LevelUp::SelectUpgrade(RunUpgrade upgrade) {
    Player* player = mGame->GetPlayer();
    if (!player) {
        Close();
        return;
    }

    // -------- STAT --------
    if (upgrade.type == UpgradeType::Stat) {
        float amount = 1.0f;

        switch (upgrade.statType) {
            case Stats::Speed:
            case Stats::Damage:
            case Stats::Area:
                amount = ADDITIONAL_MULTIPLIER_PER_UPGRADE;
                break;

            case Stats::Projectiles:
            case Stats::Regen:
            case Stats::Lucky:
            case Stats::Health:
                amount = static_cast<float>(ADDITIONAL_AMOUNT_PER_UPGRADE);
                break;

            default:
                break;
        }

        player->ApplyRunUpgrade(upgrade.statType, amount);
    }
    // -------- WEAPON --------
    else if (upgrade.type == UpgradeType::Weapon) {

        WeaponComponent* weapon =
            player->GetWeaponByType(upgrade.weaponId);

        if (weapon) {
            SDL_Log("FOi pro upgrade");
            weapon->LevelUp();
        }
        else {
            SDL_Log("NOVAAA");
            player->EquipWeapon(upgrade.weaponId);
        }
    }
    SDL_Log("Antes do close");
    Close();
    SDL_Log("Depois do close");

}