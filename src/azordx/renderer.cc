#include <azordx/renderer.h>
#include <d3dx9.h>
#include <cstdio>
#include <map>

static D3DXMATRIX transform;

namespace azordx{
extern std::map<std::string, LPDIRECT3DTEXTURE9> texture_storage;
extern std::map<std::string, D3DXVECTOR2> texture_dimensions;
extern std::map<std::string, ID3DXFont *> font_storage;
extern LPDIRECT3DDEVICE9 renderer_device;
extern LPD3DXSPRITE sprite_batch;
}

void azordx::Renderer::draw(const std::string &name, const D3DXVECTOR2 position,
                            float scale, const float rotate,
                            const D3DCOLOR color) {
  if (texture_storage.count(name)) {
    const auto &sprite_center = texture_dimensions[name] * scale * 0.5f;
    const auto &actual_pos = position - sprite_center;
    D3DXMatrixAffineTransformation2D(&transform, scale, &sprite_center,
                                     D3DXToRadian(rotate), &actual_pos);

    sprite_batch->SetTransform(&transform);
    sprite_batch->Draw(texture_storage[name], nullptr, nullptr, nullptr, color);
  }
}

void azordx::Renderer::draw(const std::string &name, const D3DXVECTOR2 position,
                            const float scale, const D3DXVECTOR2 origin,
                            const float rotate, const D3DCOLOR color) {
  if (texture_storage.count(name)) {
    const auto &actual_pos = position - origin;
    D3DXMatrixAffineTransformation2D(&transform, scale, &origin,
                                     D3DXToRadian(rotate), &actual_pos);

    sprite_batch->SetTransform(&transform);
    sprite_batch->Draw(texture_storage[name], nullptr, nullptr, nullptr, color);
  }
}

void azordx::Renderer::draw(const std::string &name, const RECT rectangle,
                            const D3DXVECTOR2 position, const float scale,
                            const float rotate, const D3DCOLOR color) {
  if (texture_storage.count(name)) {
    const auto &left = rectangle.left;
    const auto &top = rectangle.top;
    const auto &right = rectangle.right;
    const auto &bottom = rectangle.bottom;

    const auto &sprite_center = D3DXVECTOR2((right - left) * 0.5f * scale,
                                            (bottom - top) * 0.5f * scale);
    const auto &actual_pos = position - sprite_center;

    D3DXMatrixAffineTransformation2D(&transform, scale, &sprite_center,
                                     D3DXToRadian(rotate), &actual_pos);

    sprite_batch->SetTransform(&transform);
    sprite_batch->Draw(texture_storage[name], &rectangle, nullptr, nullptr,
                       color);
  }
}

void azordx::Renderer::draw(const std::string &name, const RECT rectangle,
                            const D3DXVECTOR2 position, const float scale,
                            const D3DXVECTOR2 origin, const float rotate,
                            const D3DCOLOR color) {
  if (texture_storage.count(name)) {
    const auto &actual_pos = position - origin;
    D3DXMatrixAffineTransformation2D(&transform, scale, &origin,
                                     D3DXToRadian(rotate), &actual_pos);

    sprite_batch->SetTransform(&transform);
    sprite_batch->Draw(texture_storage[name], &rectangle, nullptr, nullptr,
                       color);
  }
}

void azordx::Renderer::draw_text(const std::string &font_alias,
                                 const std::string &text, RECT rectangle,
                                 D3DCOLOR color, bool centered) {
  const auto &font = font_storage[font_alias];
  const auto format =
      centered ? (DT_CENTER | DT_VCENTER) : (DT_LEFT | DT_VCENTER);

  D3DXMatrixIdentity(&transform);
  sprite_batch->SetTransform(&transform);
  font->DrawText(sprite_batch, text.c_str(), -1, &rectangle, format, color);
}
