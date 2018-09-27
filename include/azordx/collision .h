#ifndef AZOR_COLLISION_H
#define AZOR_COLLISION_H
#include <box2d/Box2D.h>

namespace azordx {

class CollisionListener : public b2ContactListener {
 private:
  /* data */
 public:
  CollisionListener() = default;
  ~CollisionListener() = default;

 public:
  void BeginContact(b2Contact* contact) override;
  void EndContact(b2Contact* contact) override;
};

}  // namespace azordx

#endif  // AZOR_COLLISION_H