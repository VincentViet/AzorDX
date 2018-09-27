#include <azordx/object.h>
#include <azordx/scene.h>
#include <cstdio>
#include <map>
#include <vector>

static std::map<std::string, std::vector<int>> scene_storage;
static unsigned num_scene = 0;
static std::string current_scene;

namespace azordx {
void update_scene() {
  if (!current_scene.empty()) {
    const auto& scene = scene_storage[current_scene];
    if (!scene.empty()) {
      for (const auto& o : scene) ((Object*)o)->update();
    }
  }
}
void draw_scene() {
  if (!current_scene.empty()) {
    const auto& scene = scene_storage[current_scene];
    if (!scene.empty()) {
      for (const auto& o : scene) ((Object*)o)->draw();
    }
  }
}

void scene_clean_up() {
  for (auto& scene : scene_storage) {
    for (auto& o : scene.second) {
      delete (Object*)o;
    }
    scene.second.clear();
  }
}
}  // namespace azordx

void azordx::Scene::create_new_scene(const std::string& name) {
  scene_storage.insert(std::pair<std::string, std::vector<int>>(
      name, std::vector<int>{}));
}

void azordx::Scene::change_scene(const std::string& scene_name) {
  if (scene_storage.count(scene_name))
    current_scene = scene_name;
  else
    printf("\nInvalid scene id\n");
}

void azordx::Scene::add_object(const std::string& scene_name,
                               const Object* o) {
  if (scene_storage.count(scene_name)) {
    scene_storage[scene_name].push_back((int)o);
  } else
    printf("\nInvalid scene id\n");
}

void azordx::Scene::destroy_object(const Object*& o) {
  auto& scene = scene_storage[current_scene];
  for (auto it = scene.begin(); it != scene.end(); it++) {
    if ((Object*)(*it) == o) {
      delete (Object*)(*it);
      scene.erase(it);
      o = nullptr;
    }
  }
}