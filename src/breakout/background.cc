#include <breakout/background.h>
#include <azordx/renderer.h>

breakout::Background::Background(const std::string& tex_name,
                                 D3DXVECTOR2 position)
    : azordx::Object(tex_name, position) {}

    int breakout::Background::get_type(){return 0;}

    void breakout::Background::update(){}
    void breakout::Background::draw(){
        azordx::Renderer::draw(tex_name, D3DXVECTOR2(640, 400), 1.0f, 0.0f);
    }