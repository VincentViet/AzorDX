#ifndef BREAKOUT_BACKGROUND_H
#define BREAKOUT_BACKGROUND_H

#include <azordx/object.h>

namespace breakout {

class Background : public azordx::Object {
 private:
  /* data */
 public:
  explicit Background(const std::string& tex_name, D3DXVECTOR2 position);
  ~Background() = default;

  int get_type();
  void update();
  void draw();
};

}  // namespace breakout

#endif  // BREAKOUT_BACKGROUND_H