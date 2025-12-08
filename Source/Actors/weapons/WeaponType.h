//
// Created by filipe on 11/1/25.
//
#pragma once
#include <string>

// Este enum define os "IDs" para todos os tipos de armas no jogo.
enum class WeaponType
{
    MainGun,
    IceGun,
    BoomerangGun,
    LaserGun,
    SawGun,
    Aura,
    None
    // Adicione mais tipos aqui no futuro
};

inline std::string WeaponTypeToString(WeaponType type) {
    switch (type) {
        case WeaponType::IceGun:       return "Ice Gun";
        case WeaponType::LaserGun:     return "Laser Gun";
        case WeaponType::BoomerangGun: return "Boomerang";
        case WeaponType::SawGun:       return "Saw Blade";
        case WeaponType::MainGun:      return "Main Gun";
        case WeaponType::Aura:         return "Aura";
        default:                       return "Unknown Weapon";
    }
}