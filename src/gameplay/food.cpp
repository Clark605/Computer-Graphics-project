#include "gameplay/food.h"

namespace snake3d {

Food::Food() : position_{8, 10} {}

bool Food::placeFromFreeCells(const std::vector<GridPos>& freeCells, int selectorSeed) {
    if (freeCells.empty()) {
        return false;
    }

    int safeSeed = selectorSeed >= 0 ? selectorSeed : -selectorSeed;
    size_t index = static_cast<size_t>(safeSeed) % freeCells.size();
    position_ = freeCells[index];
    return true;
}

void Food::setPosition(const GridPos& position) {
    position_ = position;
}

const GridPos& Food::position() const {
    return position_;
}

}  // namespace snake3d
