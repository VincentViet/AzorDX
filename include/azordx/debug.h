#ifndef AZOR_DEBUG_H
#define AZOR_DEBUG_H

#include <Box2D/Common/b2Draw.h>
#include <string>

namespace azordx {

class Debug : public b2Draw {
 public:
  Debug();
  ~Debug();

 public:
  virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount,
                           const b2Color& color);

  /// Draw a solid closed polygon provided in CCW order.
  virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount,
                                const b2Color& color);

  /// Draw a circle.
  virtual void DrawCircle(const b2Vec2& center, float32 radius,
                          const b2Color& color);

  /// Draw a solid circle.
  virtual void DrawSolidCircle(const b2Vec2& center, float32 radius,
                               const b2Vec2& axis, const b2Color& color);

  /// Draw a line segment.
  virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2,
                           const b2Color& color);

  /// Draw a transform. Choose your own length scale.
  /// @param xf a transform.
  virtual void DrawTransform(const b2Transform& xf);

  /// Draw a point.
  virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
};
}  // namespace azordx

#endif  // AZOR_DEBUG_H