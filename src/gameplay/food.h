#pragma once

#include <vector>

#include "app/game_loop.h"

namespace snake3d {

class Food {
public:
    Food();

    bool placeFromFreeCells(const std::vector<GridPos>& freeCells, int selectorSeed);
    void setPosition(const GridPos& position);
    const GridPos& position() const;

private:
    GridPos position_;
};

}  // namespace snake3d
