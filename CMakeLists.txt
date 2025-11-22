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
        Source/Actors/Goomba.cpp
        Source/Actors/Goomba.h
        Source/Actors/Spawner.cpp
        Source/Actors/Spawner.h
        Source/Actors/Player.cpp
        Source/Actors/Player.h
        Source/Components/Drawing/AnimatorComponent.cpp
        Source/Components/Drawing/AnimatorComponent.h
        Source/Components/Drawing/RectComponent.cpp
        Source/Components/Drawing/RectComponent.h
        Source/Actors/QuestionBlock.cpp
        Source/Actors/QuestionBlock.h
        Source/Actors/Mushroom.h
        Source/Actors/Mushroom.cpp
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
        Source/Actors/weapons/boomerang/BoomerangWeapon.cpp
        Source/Actors/weapons/boomerang/BoomerangWeapon.h
        Source/Actors/weapons/boomerang/BoomerangProjectile.cpp
        Source/Actors/weapons/boomerang/BoomerangProjectile.h
        Source/Actors/weapons/ice_gun/IceProjectile.cpp
        Source/Actors/weapons/ice_gun/IceProjectile.h
        Source/Actors/weapons/ice_gun/IceGun.cpp
        Source/Actors/weapons/ice_gun/IceGun.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf OpenGL::GL)