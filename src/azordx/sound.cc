#include <azordx/sound.h>
#include <irrklang/irrKlang.h>
#include <map>

using namespace irrklang;

static std::map<std::string, ISound*> sound_storage;

namespace azordx {
extern ISoundEngine* sound_engine;
extern std::map<std::string, ISoundSource*> sound_source_storage;

void sound_clean_up() {
  for (const auto& sound : sound_storage) {
    if (sound.second) {
      sound.second->drop();
    }
  }

  if (sound_engine) {
    sound_engine->removeAllSoundSources();
    sound_engine->drop();
  }
}
}  // namespace azordx

void azordx::Sound::play(const std::string& name, bool looped) {
  ISound* sound = nullptr;
  if (!sound_storage.count(name)) {
    sound =
        sound_engine->play2D(sound_source_storage[name], looped, false, true);
    sound_storage.insert(std::pair<std::string, ISound*>(name, sound));
  } else {
    sound_engine->play2D(sound_source_storage[name], looped);
  }
}

void azordx::Sound::set_volume(const std::string& name, float value) {
  sound_storage[name]->setVolume(value);
}

void azordx::Sound::pause(const std::string& name, bool paused) {
  sound_storage[name]->setIsPaused(paused);
}

void azordx::Sound::stop(const std::string& name) {
  sound_storage[name]->stop();
}

void azordx::Sound::stop_all_sound() { sound_engine->stopAllSounds(); }