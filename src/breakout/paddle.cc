#include <breakout/paddle.h>
#include <azordx/input.h>
#include <azordx/renderer.h>

breakout::Paddle::Paddle(const std::string tag_name, const std::string tex_name,
                         const b2Shape* shape, const D3DXVECTOR2 position,
                         float angle, bool is_static, bool is_kinematic,
                         bool is_bullet, bool is_sensor, float friction,
                         float elasticity, float density)
    : Object(tag_name, tex_name, shape, position, angle, friction, elasticity,
             density, is_static, is_kinematic, is_bullet, is_sensor) {}

void breakout::Paddle::update(){
    const auto pos = b2Vec2(azordx::Input::mouse_position.x, 800);
    rigidbody->SetTransform(pos, 0.0f);
}

void breakout::Paddle::draw(){
    const auto pos = rigidbody->GetPosition();
    azordx::Renderer::draw(tex_name, D3DXVECTOR2(pos.x, pos.y), 0.3f, 0.0f);
}

void breakout::Paddle::on_collision_enter(Object* collider){}