#include <AzorDx/debug.h>
#include <d3dx9.h>
#include <cstdio>

static IDirect3DVertexBuffer9* vertex_buffer = nullptr;

namespace azordx {
extern LPDIRECT3DDEVICE9 renderer_device;

namespace debug {
// IDirect3DVertexBuffer9* vertex_buffer = nullptr;

struct DEBUGVERTEX {
  DEBUGVERTEX() {}
  DEBUGVERTEX(float x, float y, float z, D3DCOLOR c)
      : _x(x), _y(y), _z(z), color(c) {}

  float _x, _y, _z;
  D3DCOLOR color;
};
const DWORD FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

}  // namespace debug
}  // namespace azordx

azordx::Debug::Debug() {
  if (!vertex_buffer) {
    renderer_device->CreateVertexBuffer(
        sizeof(debug::DEBUGVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        debug::FVF, D3DPOOL_DEFAULT, &vertex_buffer, nullptr);
  }
}

azordx::Debug::~Debug() {
  if (vertex_buffer) {
    vertex_buffer->Release();
    vertex_buffer = nullptr;
  }
}

void azordx::Debug::DrawPolygon(const b2Vec2* vertices, int32 vertexCount,
                                const b2Color& color) {}

void azordx::Debug::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
                                     const b2Color& color) {
  debug::DEBUGVERTEX* vertex;
  vertex_buffer->Lock(0, 0, (void**)&vertex, D3DLOCK_DISCARD);

  // box case:

  vertex[0] = debug::DEBUGVERTEX(vertices[0].x, vertices[0].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));
  vertex[1] = debug::DEBUGVERTEX(vertices[1].x, vertices[1].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));
  vertex[2] = debug::DEBUGVERTEX(vertices[2].x, vertices[2].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));
  vertex[3] = debug::DEBUGVERTEX(vertices[0].x, vertices[0].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));
  vertex[4] = debug::DEBUGVERTEX(vertices[2].x, vertices[2].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));
  vertex[5] = debug::DEBUGVERTEX(vertices[3].x, vertices[3].y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 1.0f));

  vertex_buffer->Unlock();

  renderer_device->SetFVF(debug::FVF);
  renderer_device->SetStreamSource(0, vertex_buffer, 0,
                                   sizeof(debug::DEBUGVERTEX));
  renderer_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
  renderer_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}

void azordx::Debug::DrawCircle(const b2Vec2& center, float32 radius,
                               const b2Color& color) {}

void azordx::Debug::DrawSolidCircle(const b2Vec2& center, float32 radius,
                                    const b2Vec2& axis, const b2Color& color) {}

void azordx::Debug::DrawSegment(const b2Vec2& p1, const b2Vec2& p2,
                                const b2Color& color) {
  debug::DEBUGVERTEX* vertex;
  vertex_buffer->Lock(0, 0, (void**)&vertex, D3DLOCK_DISCARD);

  // box case:
  vertex[0] = debug::DEBUGVERTEX(p1.x, p1.y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 0.3f));
  vertex[1] = debug::DEBUGVERTEX(p2.x, p2.y, 0.0f,
                                 D3DXCOLOR(color.r, color.g, color.b, 0.3f));

  vertex_buffer->Unlock();

  renderer_device->SetFVF(debug::FVF);
  renderer_device->SetStreamSource(0, vertex_buffer, 0,
                                   sizeof(debug::DEBUGVERTEX));
  renderer_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
  renderer_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
  // draw debug
  renderer_device->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}

void azordx::Debug::DrawTransform(const b2Transform& xf) {}

void azordx::Debug::DrawPoint(const b2Vec2& p, float32 size,
                              const b2Color& color) {}
