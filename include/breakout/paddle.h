#ifndef BREAKOUT_PADDLE_H
#define BREAKOUT_PADDLE_H

#include <azordx/object.h>

namespace breakout {

class Paddle : public azordx::Object {
 public:
  explicit Paddle(const std::string tag_name, const std::string tex_name,
                  const b2Shape* shape,
                  const D3DXVECTOR2 position = D3DXVECTOR2(0, 0),
                  float angle = 0.0f, bool is_static = true,
                  bool is_kinematic = false, bool is_bullet = false,
                  bool is_sensor = false, float friction = 0.0f,
                  float elasticity = 1.0f, float density = 1.0f);
  ~Paddle() = default;

  void update() override;
  void draw() override;
  void on_collision_enter(Object* collider) override;
};

}  // namespace breakout

#endif  // BREAKOUT_PADDLE_H