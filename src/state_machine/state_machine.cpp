#include <state_machine/state_machine.h>

#include <iostream>

#include <GL/glut.h>

void state_machine::StateMachine::keyboard_callback(unsigned char key, int param1, int param2) {
  if (current_state)
    current_state->keyboard_callback(key, param1, param2);
  else
    std::cout << "No state set" << std::endl;
}

void state_machine::StateMachine::mouse(int button, int state, int x, int y) {
  if (current_state)
    current_state->mouse(button, state, x, y);
  else
    std::cout << "No state set" << std::endl;
}

void state_machine::StateMachine::reshape(int w, int h) {
  if (current_state)
    current_state->reshape(w, h);
  else
    std::cout << "No state set" << std::endl;
}

void state_machine::StateMachine::display() {
  if (current_state)
    current_state->display();
  else
  {
    std::cout << "No state set" << std::endl;
    glClearColor(0.0, 0.0, 0.0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
  }
}

bool state_machine::StateMachine::timeout() {
  if (current_state)
  {
    current_state->timeout();
    return true;
  }
  else
    return false;
}
