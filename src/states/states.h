#pragma once

#include <memory>

namespace states {
  enum State
  {
    CONUS_SPHERE_STAY,
    CONUS_SPHERE_MOVE,
    TOR_CYLLINDRE_STAY,
    CYLLINDRE_SPIN,
    TOR_STRETCH
  };

  class GLState
  {
   public:
    virtual GLState* display() = 0;
    virtual void timeout() = 0;
    virtual void keyboard_callback(unsigned char key, int param1, int param2);
    virtual void mouse(int button, int state, int x, int y);
    virtual void reshape(int w, int h);
  };
} // namespace states
