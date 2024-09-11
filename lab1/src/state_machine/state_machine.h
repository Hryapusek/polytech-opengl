#pragma once

#include <states/states.h>

namespace state_machine {
  class StateMachine
  {
   public:
    void keyboard_callback(unsigned char key, int param1, int param2);
    void mouse(int button, int state, int x, int y);
    void reshape(int w, int h);
    void display();
    bool timeout();

    void set_state(states::GLState* state) { current_state = state; }

    static StateMachine *instance()
    {
      static StateMachine instance;
      return &instance;
    }

   private:
    states::GLState* current_state = nullptr;

    StateMachine() = default;
  };
} // namespace state_machine
