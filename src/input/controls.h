#pragma once

#include "app/game_loop.h"

namespace snake3d {

class Controls {
public:
    Controls();

    void reset();
    void onSpecialKey(int key);
    void onKeyboard(unsigned char key);

    bool consumePendingDirection(Direction& outDirection);
    bool consumePauseToggleRequested();
    bool consumeRestartRequested();

private:
    bool hasPendingDirection_;
    Direction pendingDirection_;
    bool pauseToggleRequested_;
    bool restartRequested_;
};

}  // namespace snake3d
