#include "gameplay/arena.h"

namespace snake3d {

Arena::Arena(int width, int height) : width_(width), height_(height) {}

int Arena::width() const {
    return width_;
}

int Arena::height() const {
    return height_;
}

bool Arena::isInside(const GridPos& position) const {
    return position.x >= 0 && position.y >= 0 && position.x < width_ && position.y < height_;
}

std::vector<GridPos> Arena::collectFreeCells(const std::vector<GridPos>& occupied) const {
    std::vector<GridPos> freeCells;
    freeCells.reserve(static_cast<size_t>(width_ * height_));

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            GridPos probe{x, y};
            bool used = false;
            for (const GridPos& cell : occupied) {
                if (cell == probe) {
                    used = true;
                    break;
                }
            }
            if (!used) {
                freeCells.push_back(probe);
            }
        }
    }

    return freeCells;
}

GridPos Arena::findFirstFreeCell(const std::vector<GridPos>& occupied) const {
    std::vector<GridPos> freeCells = collectFreeCells(occupied);
    if (!freeCells.empty()) {
        return freeCells.front();
    }

    return GridPos{0, 0};
}

}  // namespace snake3d
