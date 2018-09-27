#include <breakout/edge.h>

breakout::Edge::Edge(const std::string tag_name, const std::string tex_name,
                     const b2Shape* shape, const D3DXVECTOR2 position,
                     float angle, bool is_static, bool is_kinematic,
                     bool is_bullet, bool is_sensor, float friction,
                     float elasticity, float density)
    : Object(tag_name, tex_name, shape, position, angle, friction, elasticity,
             density, is_static, is_kinematic, is_bullet, is_sensor) {}

void breakout::Edge::on_collision_enter(Object* collider){
    // printf("%s", collider->get_tag().c_str());
}