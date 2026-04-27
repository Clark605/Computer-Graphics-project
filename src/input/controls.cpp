#include "input/controls.h"

#include <gl/glut.h>

namespace snake3d {

Controls::Controls() {
    reset();
}

void Controls::reset() {
    hasPendingDirection_ = false;
    pendingDirection_ = Direction::RIGHT;
    pauseToggleRequested_ = false;
    restartRequested_ = false;
}

void Controls::onSpecialKey(int key) {
    switch (key) {
        case GLUT_KEY_UP:
            pendingDirection_ = Direction::UP;
            hasPendingDirection_ = true;
            break;
        case GLUT_KEY_DOWN:
            pendingDirection_ = Direction::DOWN;
            hasPendingDirection_ = true;
            break;
        case GLUT_KEY_LEFT:
            pendingDirection_ = Direction::LEFT;
            hasPendingDirection_ = true;
            break;
        case GLUT_KEY_RIGHT:
            pendingDirection_ = Direction::RIGHT;
            hasPendingDirection_ = true;
            break;
    }
}

void Controls::onKeyboard(unsigned char key) {
    if (key == 'p' || key == 'P') {
        pauseToggleRequested_ = true;
    } else if (key == 'r' || key == 'R') {
        restartRequested_ = true;
    }
}

bool Controls::consumePendingDirection(Direction& outDirection) {
    if (!hasPendingDirection_) {
        return false;
    }

    outDirection = pendingDirection_;
    hasPendingDirection_ = false;
    return true;
}

bool Controls::consumePauseToggleRequested() {
    bool value = pauseToggleRequested_;
    pauseToggleRequested_ = false;
    return value;
}

bool Controls::consumeRestartRequested() {
    bool value = restartRequested_;
    restartRequested_ = false;
    return value;
}

}  // namespace snake3d
