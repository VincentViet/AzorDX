#ifndef AZORDX_OBJECT_H
#define AZORDX_OBJECT_H

#include <Box2D/Box2D.h>
#include <d3dx9math.h>
#include <string>

namespace azordx {

class Object {
 protected:
  b2Body* rigidbody;
  const std::string tag_name;
  std::string tex_name;

 public:
  explicit Object(const std::string& tex_name, D3DXVECTOR2 position);
  explicit Object(const std::string& tag_name, const std::string& tex_name,
                  const b2Shape* shape,
                  const D3DXVECTOR2 position = D3DXVECTOR2(0, 0),
                  float angle = 0.0f, float friction = 0.3f,
                  float elasticity = 1.0f, float density = 1.0f,
                  bool is_static = true, bool is_kinematic = false,
                  bool is_bullet = false, bool is_sensor = false);
  virtual ~Object();

 public:
  virtual int get_type();
  virtual void update();
  virtual void draw();
  virtual void on_collision_enter(Object* collider);
  virtual void on_collision_exit(Object* collider);

 public:
  void apply_force(const D3DXVECTOR2 force);
  void apply_torque(float torque);
  void apply_angular_impulse(float impulse);
  void apply_linear_impulse(D3DXVECTOR2 impulse, D3DXVECTOR2 point);
  float get_mass() const;
  D3DXVECTOR2 get_position() const;
  const std::string get_tag() const;
  void set_active(bool flag);
};

}  // namespace azordx
#endif  // AZORDX_OBJECT_H