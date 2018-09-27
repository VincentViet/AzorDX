#ifndef BREAKOUT_BALL_H
#define BREAKOUT_BALL_H

#include <azordx/object.h>

namespace breakout {

class Ball : public azordx::Object {
 public:
  explicit Ball(const std::string tag_name, const std::string tex_name,
                const b2Shape* shape,
                const D3DXVECTOR2 position = D3DXVECTOR2(0, 0),
                float angle = 0.0f, bool is_static = true,
                bool is_kinematic = false, bool is_bullet = false,
                bool is_sensor = false, float friction = 0.0f,
                float elasticity = 1.0f, float density = 0.0f);
  ~Ball() = default;

  void draw() override;
  void on_collision_enter(Object* collider) override;
};

}  // namespace breakout

#endif  // BREAKOUT_BALL_H