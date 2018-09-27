#ifndef AZORDX_SCENE_H
#define AZORDX_SCENE_H

#include <string>

namespace azordx {

class Scene {
 private:
  /* data */
 public:
  Scene() = delete;
  ~Scene() = delete;

  static void create_new_scene(const std::string& name);
  static void change_scene(const std::string& scene_name);
  static void add_object(const std::string& scene_name, const Object* o);
  static void destroy_object(const Object*& o);
};

}  // namespace azordx

#endif  // AZORDX_SCENE_H