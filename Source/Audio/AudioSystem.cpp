#include "AudioSystem.h"
#include <filesystem>
#include "SDL.h"
#include "SDL_mixer.h"

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels) {
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  Mix_AllocateChannels(numChannels);
  Mix_Volume(-1, MIX_MAX_VOLUME / 4);  // Set default volume to 25%
  mChannels.resize(numChannels);
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem() {
  for (auto& sound : mSounds) {
    Mix_FreeChunk(sound.second);
  }
  mSounds.clear();

  Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime) {
  for (int i = 0; i < mChannels.size(); i++) {
    if (mChannels[i].IsValid()) {
      if (!Mix_Playing(i)) {
        mHandleMap.erase(mChannels[i]);
        mChannels[i].Reset();
      }
    }
  }
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping) {
  // 1) Carrega (ou pega do cache) o Mix_Chunk
  Mix_Chunk* sound = GetSound(soundName);
  if (!sound) {
    SDL_Log("[AudioSystem] PlaySound couldn't find sound for %s",
            soundName.c_str());
    return SoundHandle::Invalid;
  }

  // 2) Escolhe canal livre
  int channel = -1;

  // Primeiro: procura canal totalmente livre
  for (int i = 0; i < static_cast<int>(mChannels.size()); ++i) {
    if (!mChannels[i].IsValid()) {
      channel = i;
      break;
    }
  }

  // Se nenhum canal livre, aplica política: roubar um som NÃO-looping
  if (channel == -1) {
    for (int i = 0; i < static_cast<int>(mChannels.size()); ++i) {
      SoundHandle handle = mChannels[i];
      auto it = mHandleMap.find(handle);
      if (it != mHandleMap.end() && !it->second.mIsLooping) {
        // Para o som atual desse canal
        Mix_HaltChannel(i);

        // Remove handle antigo
        mHandleMap.erase(it);
        mChannels[i].Reset();

        channel = i;
        break;
      }
    }
  }

  // Se ainda assim não achou canal, desiste
  if (channel == -1) {
    SDL_Log("[AudioSystem] No free channel to play %s", soundName.c_str());
    return SoundHandle::Invalid;
  }

  // 3) Configura número de loops
  int loops = looping ? -1 : 0;

  // 4) Gera novo handle
  ++mLastHandle;
  SoundHandle handle = mLastHandle;

  // 5) Atualiza mChannels e mHandleMap antes de tocar
  mChannels[channel] = handle;

  HandleInfo hi;
  hi.mSoundName = soundName;
  hi.mChannel = channel;
  hi.mIsLooping = looping;
  hi.mIsPaused = false;

  mHandleMap.emplace(handle, hi);

  // 6) Toca o som
  if (Mix_PlayChannel(channel, sound, loops) == -1) {
    SDL_Log("[AudioSystem] Mix_PlayChannel failed for %s: %s",
            soundName.c_str(), Mix_GetError());
    // Reverte estado
    mHandleMap.erase(handle);
    mChannels[channel].Reset();
    return SoundHandle::Invalid;
  }

  return handle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound) {
  auto it = mHandleMap.find(sound);
  if (it == mHandleMap.end()) {
    SDL_Log("[AudioSystem] StopSound couldn't find handle %s",
            sound.GetDebugStr());
    return;
  }

  int channel = it->second.mChannel;
  Mix_HaltChannel(channel);
  mHandleMap.erase(it);
  mChannels[channel].Reset();
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound) {
  if (mHandleMap.find(sound) == mHandleMap.end()) {
    SDL_Log("[AudioSystem] PauseSound couldn't find handle %s",
            sound.GetDebugStr());
    return;
  }

  if (!mHandleMap[sound].mIsPaused) {
    Mix_Pause(mHandleMap[sound].mChannel);
    mHandleMap[sound].mIsPaused = true;
  }
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound) {
  if (mHandleMap.find(sound) == mHandleMap.end()) {
    SDL_Log("[AudioSystem] ResumeSound couldn't find handle %s",
            sound.GetDebugStr());
    return;
  }

  if (mHandleMap[sound].mIsPaused) {
    Mix_Resume(mHandleMap[sound].mChannel);
    mHandleMap[sound].mIsPaused = false;
  }
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound) {
  if (mHandleMap.find(sound) == mHandleMap.end()) {
    return SoundState::Stopped;
  }

  if (mHandleMap[sound].mIsPaused) {
    return SoundState::Paused;
  }

  return SoundState::Playing;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds() {
  Mix_HaltChannel(-1);

  for (auto& mChannel : mChannels) {
    mChannel.Reset();
  }

  mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds() {
#ifndef __clang_analyzer__
  std::error_code ec{};
  for (const auto& rootDirEntry :
       std::filesystem::directory_iterator{"Assets/Sounds", ec}) {
    std::string extension = rootDirEntry.path().extension().string();
    if (extension == ".ogg" || extension == ".wav") {
      std::string fileName = rootDirEntry.path().stem().string();
      fileName += extension;
      CacheSound(fileName);
    }
  }
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string& soundName) {
  GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk* AudioSystem::GetSound(const std::string& soundName) {
  std::string fileName = "../Assets/Sounds/";
  fileName += soundName;

  Mix_Chunk* chunk = nullptr;
  auto iter = mSounds.find(fileName);
  if (iter != mSounds.end()) {
    chunk = iter->second;
  } else {
    chunk = Mix_LoadWAV(fileName.c_str());
    if (!chunk) {
      SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
      return nullptr;
    }

    mSounds.emplace(fileName, chunk);
  }
  return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8* keyState) {
  // Debugging code that outputs all active sounds on leading edge of period key
  if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey) {
    SDL_Log("[AudioSystem] Active Sounds:");
    for (size_t i = 0; i < mChannels.size(); i++) {
      if (mChannels[i].IsValid()) {
        auto iter = mHandleMap.find(mChannels[i]);
        if (iter != mHandleMap.end()) {
          HandleInfo& hi = iter->second;
          SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
                  static_cast<unsigned>(i), mChannels[i].GetDebugStr(),
                  hi.mSoundName.c_str(), hi.mIsLooping, hi.mIsPaused);
        } else {
          SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
                  mChannels[i].GetDebugStr());
        }
      }
    }
  }

  mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}
