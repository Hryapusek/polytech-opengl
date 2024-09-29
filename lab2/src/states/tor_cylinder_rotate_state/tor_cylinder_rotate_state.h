#pragma once

#include <constants.h>
#include <states/states.h>

namespace states {
  class TorCylinderRotateState : public GLState
  {
   public:
    GLState* display() override final;
    void timeout() override final;

   private:
    size_t frames_count = 0;
  };
} // namespace states
