#include <breakout/background.h>
#include <breakout/paddle.h>
#include <breakout/ball.h>
#include <breakout/edge.h>
#include <azordx/scene.h>
#include <azordx/resources.h>
#include <azordx/sound.h>

using namespace breakout;

Paddle* player;
Background* bg;
Ball* my_ball;
Edge* edge_top;
Edge* edge_right;
Edge* edge_bottom;
Edge* edge_left;

void load_resources() {
  azordx::Resources::load_texture_from_file("background.jpg", "bg");
  azordx::Resources::load_texture_from_file("paddle.png", "paddle");
  azordx::Resources::load_texture_from_file("circle.png", "ball");
  
  azordx::Resources::load_sound_from_file("solid.wav", "solid");
  azordx::Resources::load_sound_from_file("breakout.mp3", "bk");
}

void initialize() {
  b2PolygonShape paddle_shape;
  paddle_shape.SetAsBox(76.8f, 19.2f);
  player = new Paddle("paddle", "paddle", &paddle_shape);
  bg = new Background("bg", D3DXVECTOR2(640, 400));

  b2EdgeShape edge_shape;
  edge_shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(1280.0f, 0.0f));
  edge_top = new Edge("edge-top", "", &edge_shape, D3DXVECTOR2(0.0f, 0.0f));
  edge_bottom = new Edge("edge-top", "", &edge_shape, D3DXVECTOR2(0.0f, 800.0f));

  edge_shape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, 800.0f));
  edge_left = new Edge("edge-top", "", &edge_shape, D3DXVECTOR2(0.0f, 0.0f));
  edge_right = new Edge("edge-top", "", &edge_shape, D3DXVECTOR2(1280.0f, 0.0f));

  b2CircleShape ball_shape;
  ball_shape.m_radius = 20.0f;
  my_ball = new Ball("awesome-ball", "ball", &ball_shape, D3DXVECTOR2(640, 400), 0.0f, false);
  my_ball->apply_force(D3DXVECTOR2(500.0f, -1000.0f));

  azordx::Scene::create_new_scene("level 1");
  azordx::Scene::add_object("level 1", bg);
  azordx::Scene::add_object("level 1", player);
  azordx::Scene::add_object("level 1", my_ball);
  azordx::Scene::change_scene("level 1");

  azordx::Sound::play("bk", true);

}
void clean_up() {}
void on_left_mouse_btn_click() {}
void on_left_mouse_btn_dbclick() {}
void on_key_down(int key) {}