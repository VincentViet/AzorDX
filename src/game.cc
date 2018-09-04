#include <AzorDx/renderer.h>
#include <AzorDx/resources.h>
#include <AzorDx/sound.h>

using namespace azordx;

void initialize() {
  Renderer::initialize();
  Resources::initialize();
  Sound::initialize();
}

void load_resources() {
  Resources::load_texture_from_file("fps.png", "fps");
  Resources::load_sound_from_file("age.mp3", "age");

  Sound::play("age");
}

void update() {}

void on_left_mouse_btn_click() {}

void on_left_mouse_btn_dbclick() {}

void on_key_down(int key) {
  // switch (key) {}
}

void draw() {
  Renderer::draw("fps", D3DXVECTOR2(400, 300), 0.2f, 0);
}

void clean_up() {
  Renderer::clean_up();
  Resources::clean_up();
  Sound::clean_up();
}