#include <breakout/ball.h>
#include <azordx/renderer.h>
#include <azordx/sound.h>

breakout::Ball::Ball(const std::string tag_name, const std::string tex_name,
                     const b2Shape* shape, const D3DXVECTOR2 position,
                     float angle, bool is_static, bool is_kinematic,
                     bool is_bullet, bool is_sensor, float friction,
                     float elasticity, float density)
    : Object(tag_name, tex_name, shape, position, angle, friction, elasticity,
             density, is_static, is_kinematic, is_bullet, is_sensor) {}


void breakout::Ball::draw(){
    b2Vec2 pos = this->rigidbody->GetPosition();
    azordx::Renderer::draw(tex_name, D3DXVECTOR2(pos.x, pos.y), 0.05f, 0.0f);
}

void breakout::Ball::on_collision_enter(Object* collider){
    azordx::Sound::play("solid");
}