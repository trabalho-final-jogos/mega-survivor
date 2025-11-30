cmake_minimum_required(VERSION 3.26)
# Configurando o padrão c++
set(CMAKE_CXX_STANDARD 17)

# Configurando o nome e executável do projeto
set(PROJECT_NAME "mega-survivor")
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
        Source/Renderer/Shader.cpp
        Source/Renderer/Shader.h
        Source/Main.cpp
        Source/Math.cpp
        Source/Math.h
        Source/Random.cpp
        Source/Random.h
        Source/Game.cpp
        Source/Game.h
        Source/Actors/Actor.cpp
        Source/Actors/Actor.h
        Source/Components/Component.cpp
        Source/Components/Component.h
        Source/Components/Drawing/DrawComponent.cpp
        Source/Components/Drawing/DrawComponent.h
        Source/Components/Physics/RigidBodyComponent.cpp
        Source/Components/Physics/RigidBodyComponent.h
        Source/Components/Physics/AABBColliderComponent.cpp
        Source/Components/Physics/AABBColliderComponent.h
        Source/Components/ParticleSystemComponent.cpp
        Source/Components/ParticleSystemComponent.h
        Source/Renderer/Renderer.cpp
        Source/Renderer/Renderer.h
        Source/Renderer/VertexArray.h
        Source/Renderer/VertexArray.cpp
        Source/Renderer/Texture.cpp
        Source/Renderer/Texture.h
        Source/Actors/Block.cpp
        Source/Actors/Block.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/Actors/Player.cpp
        Source/Actors/Player.h
        Source/Components/Drawing/AnimatorComponent.cpp
        Source/Components/Drawing/AnimatorComponent.h
        Source/Components/Drawing/RectComponent.cpp
        Source/Components/Drawing/RectComponent.h
        Source/Actors/Aim.cpp
        Source/Actors/Aim.h
        Source/Actors/weapons/Projectile.h
        Source/Actors/weapons/Projectile.cpp
        Source/Components/WeaponComponent.h
        Source/Actors/weapons/main_gun/MainGun.h
        Source/Actors/weapons/main_gun/Bullet.h
        Source/Actors/weapons/main_gun/Bullet.cpp
        Source/Actors/weapons/main_gun/MainGun.cpp
        Source/Components/WeaponComponent.cpp
        Source/Actors/weapons/WeaponType.h
        Source/Components/ProjectilePoolComponent.cpp
        Source/Components/ProjectilePoolComponent.h
        Source/Actors/weapons/boomerang/BoomerangGun.cpp
        Source/Actors/weapons/boomerang/BoomerangGun.h
        Source/Actors/weapons/boomerang/BoomerangProjectile.cpp
        Source/Actors/weapons/boomerang/BoomerangProjectile.h
        Source/Actors/weapons/ice_gun/IceProjectile.cpp
        Source/Actors/weapons/ice_gun/IceProjectile.h
        Source/Actors/weapons/ice_gun/IceGun.cpp
        Source/Actors/weapons/ice_gun/IceGun.h
        Source/UI/UIButton.cpp
        Source/UI/UIButton.h
        Source/UI/UIElement.h
        Source/UI/UIElement.cpp
        Source/UI/UIImage.cpp
        Source/UI/UIImage.h
        Source/UI/UIRect.cpp
        Source/UI/UIRect.h
        Source/UI/UIText.cpp
        Source/UI/UIText.h
        Source/UI/Screens/UIScreen.cpp
        Source/UI/Screens/UIScreen.h
        Source/UI/Screens/MainMenu.cpp
        Source/UI/Screens/MainMenu.h
        Source/UI/Screens/PausedMenu.cpp
        Source/UI/Screens/PausedMenu.h
        Source/UI/Screens/HUD.cpp
        Source/UI/Screens/HUD.h
        Source/UI/Screens/GameOver.cpp
        Source/UI/Screens/GameOver.h
        Source/Audio/AudioSystem.cpp
        Source/Audio/AudioSystem.h
        Source/Renderer/Font.cpp
        Source/Renderer/Font.h
        Source/UI/Screens/Level1.cpp
        Source/UI/Screens/Level1.h
        Source/UI/Screens/MetaProg.cpp
        Source/UI/Screens/MetaProg.h
        Source/Managers/UpgradeManager.cpp
        Source/Managers/UpgradeManager.h
        Source/Actors/weapons/laser_beam/LaserGun.cpp
        Source/Actors/weapons/laser_beam/LaserGun.h
        Source/Actors/weapons/saw_blade/SawGun.cpp
        Source/Actors/weapons/saw_blade/SawGun.h
        Source/Actors/weapons/saw_blade/SawProjectile.h
        Source/Actors/weapons/saw_blade/SawProjectile.cpp
        Source/Actors/weapons/laser_beam/LaserProjectile.cpp
        Source/Actors/weapons/laser_beam/LaserProjectile.h
        Source/Actors/weapons/aura/AuraWeapon.cpp
        Source/Actors/weapons/aura/AuraWeapon.h
        Source/Actors/weapons/aura/AuraActor.cpp
        Source/Actors/weapons/aura/AuraActor.h
        Source/UI/Screens/CharSelection.cpp
        Source/UI/Screens/CharSelection.h
        Source/Actors/enemies/Enemy1.cpp
        Source/Actors/enemies/Enemy1.h
        Source/Actors/enemies/EnemyType.h
        Source/Actors/enemies/Bat.cpp
        Source/Actors/enemies/Bat.h
        Source/Color.cpp
        Source/Color.h
        Source/Managers/ColorPalette.cpp
        Source/Managers/ColorPalette.h
        Source/Actors/Enemy.cpp
        Source/Actors/Enemy.h
        Source/Actors/XPGem.cpp
        Source/Actors/XPGem.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf OpenGL::GL)