#pragma once

#include <vector>

#include "app/game_loop.h"

namespace snake3d {

class Snake {
public:
    Snake();

    void reset();
    void tryQueueDirection(Direction direction);
    void applyQueuedDirection();
    void stepForward();
    void growByOne();

    bool isSelfCollision() const;

    const GridPos& head() const;
    const std::vector<GridPos>& segments() const;
    Direction direction() const;

private:
    GridPos nextHeadFor(Direction direction) const;
    static bool isReverse(Direction current, Direction next);

    std::vector<GridPos> segments_;
    Direction direction_;
    bool hasQueuedDirection_;
    Direction queuedDirection_;
    bool growOnNextStep_;
};

}  // namespace snake3d
