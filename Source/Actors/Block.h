//
// Created by Lucas N. Ferreira on 28/09/23.
//

#pragma once

#include "Actor.h"

class Block : public Actor
{
public:
    explicit Block(Game* game, const std::string &texturePath, std::string name = "");
    virtual void StartBounce() ;
    void OnUpdate(float deltaTime) override;

protected:
    Vector2 mOriginalPosition;
    bool mIsBouncing;
    float mBounceTimer;
    bool mCanBounce;
};
