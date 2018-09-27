#include <AzorDx/object.h>

using namespace azordx;

namespace azordx {
extern b2World* physics_engine;
}

Object::Object(const std::string& tex_name, D3DXVECTOR2 position)
  : tex_name(tex_name), tag_name(){
    rigidbody = nullptr;
}

Object::Object(const std::string& tag_name, const std::string& tex_name,
               const b2Shape* shape, const D3DXVECTOR2 position, float angle,
               float friction, float elasticity, float density, bool is_static,
               bool is_kinematic, bool is_bullet, bool is_sensor)
    : tag_name(tag_name), tex_name(tex_name){
  if (physics_engine) {
    b2BodyDef body_def;
    b2FixtureDef fixture_def;

    if (is_bullet) {
      body_def.bullet = is_bullet;
    } else {
      if (is_kinematic) {
        body_def.type = b2_kinematicBody;
      } else if (is_static) {
        body_def.type = b2_staticBody;
      } else
        body_def.type = b2_dynamicBody;
    }

    body_def.userData = this;
    this->rigidbody = physics_engine->CreateBody(&body_def);
    this->rigidbody->SetTransform(b2Vec2(position.x, position.y), angle * b2DegToRad);

    fixture_def.density = density;
    fixture_def.friction = friction;
    fixture_def.isSensor = is_sensor;
    fixture_def.restitution = elasticity;
    fixture_def.shape = shape;
    fixture_def.userData = this;
    rigidbody->CreateFixture(&fixture_def);
  }
}

Object::~Object() {
  if (rigidbody) physics_engine->DestroyBody(this->rigidbody);
}


int Object::get_type(){return 0;}

void Object::update(){}

void Object::draw(){}

void Object::on_collision_enter(Object* collider) {}

void Object::on_collision_exit(Object* collider) {}

void Object::apply_force(D3DXVECTOR2 force) {
  this->rigidbody->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void Object::apply_torque(const float torque) {
  this->rigidbody->ApplyTorque(torque, true);
}

void Object::apply_angular_impulse(const float impulse) {
  this->rigidbody->ApplyAngularImpulse(impulse, true);
}

void Object::apply_linear_impulse(const D3DXVECTOR2 impulse,
                                  const D3DXVECTOR2 point) {
  this->rigidbody->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y),
                                      b2Vec2(point.x, point.y), true);
}

float Object::get_mass() const { return this->rigidbody->GetMass(); }

D3DXVECTOR2 Object::get_position() const {
  const auto pos = this->rigidbody->GetPosition();
  return D3DXVECTOR2(pos.x, pos.y);
}

const std::string Object::get_tag() const { return this->tag_name; }

void Object::set_active(bool flag) { this->rigidbody->SetActive(flag); }