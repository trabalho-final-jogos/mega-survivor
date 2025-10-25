//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "DrawComponent.h"
#include "../../Game.h"
#include "../../Actors/Actor.h"

DrawComponent::DrawComponent(class Actor *owner, int drawOrder)
        : Component(owner)
        , mDrawOrder(drawOrder)
        , mIsVisible(true)
        , mColor(Vector3::One)
{
    mOwner->GetGame()->AddDrawable(this);
}

DrawComponent::~DrawComponent()
{
    mOwner->GetGame()->RemoveDrawable(this);
}

void DrawComponent::Draw(Renderer *renderer)
{
    // TEMPORARY: Draw a red rectangle for testing if (!mIsVisible) return;

    if (!mIsVisible) return;

    Vector2 worldPos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();
    Vector2 screenPos = worldPos - cameraPos;

    SDL_Log("DRAW - World: (%.1f, %.1f), Screen: (%.1f, %.1f)",
            worldPos.x, worldPos.y, screenPos.x, screenPos.y);


    Vector2 size(32.0f, 32.0f);
    Vector3 color(1.0f, 0.0f, 0.0f);

    renderer->DrawRect(
        worldPos,       // Posição mundial
        size,
        0.0f,
        color,
        cameraPos,      // Posição da câmera
        RendererMode::TRIANGLES
    );

    SDL_Log("DrawRect called with worldPos + cameraPos");

}
