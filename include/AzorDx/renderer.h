#ifndef AZORDX_RENDERER_H
#define AZORDX_RENDERER_H

#include <d3dx9math.h>
#include <windows.h>
#include <string>

namespace azordx {
class Renderer {
 private:
 public:
  Renderer() = delete;
  ~Renderer() = delete;

 public:
  static void draw(const std::string& name, D3DXVECTOR2 position, float scale,
                   float rotate, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

  static void draw(const std::string& name, D3DXVECTOR2 position, float scale,
                   D3DXVECTOR2 origin, float rotate,
                   D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

  static void draw(const std::string& name, RECT rectangle,
                   D3DXVECTOR2 position, float scale, float rotate,
                   D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

  static void draw(const std::string& name, RECT rectangle,
                   D3DXVECTOR2 position, float scale, D3DXVECTOR2 origin,
                   float rotate, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

  static void draw_text(const std::string& font_alias, const std::string& text,
                        RECT rectangle, D3DCOLOR color, bool centered = true);
};
}  // namespace azordx

#endif  // AZORDX_RENDERER_H