#include "gameplay/snake.h"

namespace snake3d {

Snake::Snake() {
    reset();
}

void Snake::reset() {
    segments_.clear();
    segments_.push_back(GridPos{4, 10});
    segments_.push_back(GridPos{3, 10});
    segments_.push_back(GridPos{2, 10});

    direction_ = Direction::RIGHT;
    hasQueuedDirection_ = false;
    queuedDirection_ = Direction::RIGHT;
    growOnNextStep_ = false;
}

void Snake::tryQueueDirection(Direction direction) {
    if (hasQueuedDirection_) {
        return;
    }

    if (isReverse(direction_, direction)) {
        return;
    }

    queuedDirection_ = direction;
    hasQueuedDirection_ = true;
}

void Snake::applyQueuedDirection() {
    if (hasQueuedDirection_) {
        direction_ = queuedDirection_;
        hasQueuedDirection_ = false;
    }
}

void Snake::stepForward() {
    GridPos newHead = nextHeadFor(direction_);

    segments_.insert(segments_.begin(), newHead);
    if (growOnNextStep_) {
        growOnNextStep_ = false;
        return;
    }

    segments_.pop_back();
}

void Snake::growByOne() {
    growOnNextStep_ = true;
}

bool Snake::isSelfCollision() const {
    const GridPos& h = head();
    for (size_t i = 1; i < segments_.size(); ++i) {
        if (segments_[i] == h) {
            return true;
        }
    }
    return false;
}

const GridPos& Snake::head() const {
    return segments_[0];
}

const std::vector<GridPos>& Snake::segments() const {
    return segments_;
}

Direction Snake::direction() const {
    return direction_;
}

GridPos Snake::nextHeadFor(Direction direction) const {
    GridPos h = head();
    switch (direction) {
        case Direction::UP:
            --h.y;
            break;
        case Direction::DOWN:
            ++h.y;
            break;
        case Direction::LEFT:
            --h.x;
            break;
        case Direction::RIGHT:
            ++h.x;
            break;
    }
    return h;
}

bool Snake::isReverse(Direction current, Direction next) {
    return (current == Direction::UP && next == Direction::DOWN) ||
           (current == Direction::DOWN && next == Direction::UP) ||
           (current == Direction::LEFT && next == Direction::RIGHT) ||
           (current == Direction::RIGHT && next == Direction::LEFT);
}

}  // namespace snake3d
