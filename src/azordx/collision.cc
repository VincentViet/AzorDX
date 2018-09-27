#include <azordx/collision .h>
#include <azordx/object.h>

void azordx::CollisionListener::BeginContact(b2Contact* contact) {
  if (contact->IsTouching()) {
    const auto fixture_a = contact->GetFixtureA();
    const auto fixture_b = contact->GetFixtureB();

    Object* o_a = static_cast<Object*>(fixture_a->GetUserData());
    Object* o_b = static_cast<Object*>(fixture_b->GetUserData());

    o_a->on_collision_enter(o_b);
    o_b->on_collision_enter(o_a);
  }
}

void azordx::CollisionListener::EndContact(b2Contact* contact) {
  const auto fixture_a = contact->GetFixtureA();
  const auto fixture_b = contact->GetFixtureB();

  Object* o_a = static_cast<Object*>(fixture_a->GetUserData());
  Object* o_b = static_cast<Object*>(fixture_b->GetUserData());

  o_a->on_collision_exit(o_b);
  o_b->on_collision_exit(o_a);
}