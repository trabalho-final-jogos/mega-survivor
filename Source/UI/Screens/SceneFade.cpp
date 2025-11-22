// SceneFade.cpp
#include "SceneFade.h"
#include "../UIRect.h"

SceneFade::SceneFade(Game* game, GameScene nextScene, float duration)
    : UIScreen(game, "../Assets/Fonts/Arial.ttf"),
      mNextScene(nextScene),
      mDuration(duration),
      mElapsed(0.0f),
      mSceneSwitched(false),
      mFadeRect(nullptr) {
  // Retângulo preto cobrindo a tela inteira, no topo de tudo
  const float w = 1024.0f;
  const float h = 768.0f;

  mFadeRect = AddRect(Vector2(0.0f, 0.0f), Vector2(w, h), 1.0f, 0.0f,
                      1000);  // drawOrder bem alto

  // Começa totalmente transparente
  mFadeRect->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
}

void SceneFade::Update(float deltaTime) {
  UIScreen::Update(deltaTime);

  mElapsed += deltaTime;
  if (mElapsed > mDuration)
    mElapsed = mDuration;

  const float half = mDuration * 0.5f;

  float alpha = 0.0f;

  if (mElapsed <= half) {
    // Fase 1: fade-out (0 -> 1)
    alpha = mElapsed / half;
  } else {
    // Fase 2: fade-in (1 -> 0)
    alpha = 1.0f - (mElapsed - half) / half;
  }

  if (mFadeRect) {
    mFadeRect->SetColor(Vector4(0.7f, 0.7f, 0.7f, alpha));
  }

  // No meio do efeito, troca de cena + zera sons (uma única vez)
  if (!mSceneSwitched && mElapsed >= half) {
    mSceneSwitched = true;

    // Para todos os sons atuais
    if (mGame->GetAudio()) {
      mGame->GetAudio()->StopAllSounds();
    }

    // Troca efetiva de cena
    mGame->SetScene(mNextScene);

    // Se quiser iniciar música da nova cena aqui, também pode
    if (mNextScene == GameScene::Level1 && mGame->GetAudio()) {
      mGame->GetAudio()->PlaySound("Music.ogg", true);
    }
  }

  // Ao final do efeito, fecha este UIScreen
  if (mElapsed >= mDuration) {
    Close();
  }
}
