#pragma once

#include <vector>

#include "app/game_loop.h"

namespace snake3d {

class Arena {
public:
    Arena(int width, int height);

    int width() const;
    int height() const;

    bool isInside(const GridPos& position) const;
    std::vector<GridPos> collectFreeCells(const std::vector<GridPos>& occupied) const;
    GridPos findFirstFreeCell(const std::vector<GridPos>& occupied) const;

private:
    int width_;
    int height_;
};

}  // namespace snake3d
