#include "app/game_loop.h"

#include <gl/glut.h>

#include "gameplay/arena.h"
#include "gameplay/food.h"
#include "gameplay/snake.h"
#include "input/controls.h"
#include "render/renderer.h"

namespace snake3d {

GameLoop::GameLoop(Arena& arena, Snake& snake, Food& food, Controls& controls, Renderer& renderer)
    : arena_(arena),
      snake_(snake),
      food_(food),
      controls_(controls),
      renderer_(renderer),
      state_(GameState::START),
      stateBeforePause_(GameState::RUNNING),
      score_(0),
      speedTier_(0),
      stepIntervalMs_(GameConfig::kBaseStepMs),
      lastStepAtMs_(0),
      lastPauseToggleAtMs_(-1000),
      lastRestartAtMs_(-1000) {}

void GameLoop::initialize() {
    state_ = GameState::START;
    stateBeforePause_ = GameState::RUNNING;
    score_ = 0;
    speedTier_ = 0;
    stepIntervalMs_ = GameConfig::kBaseStepMs;
    lastStepAtMs_ = 0;
    lastPauseToggleAtMs_ = -1000;
    lastRestartAtMs_ = -1000;

    snake_.reset();
    controls_.reset();
    placeFood();
}

void GameLoop::update(int nowMs) {
    // Only apply direction input in START or RUNNING states
    if (state_ == GameState::START || state_ == GameState::RUNNING) {
        applyDirectionInput();
    }

    // Only process game step when actively running (not paused, not at start, not game over/win)
    if (state_ == GameState::RUNNING && nowMs - lastStepAtMs_ >= stepIntervalMs_) {
        lastStepAtMs_ = nowMs;
        stepRunningState();
    }

    renderer_.setFrameState(state_, score_, snake_, food_, arena_);
}

void GameLoop::render() const {
    renderer_.drawFrame();
}

void GameLoop::onSpecialKey(int key) {
    controls_.onSpecialKey(key);
}

void GameLoop::onKeyboard(unsigned char key) {
    controls_.onKeyboard(key);

    // Debounce restart: only process once per 100ms
    int nowMs = glutGet(GLUT_ELAPSED_TIME);
    if (controls_.consumeRestartRequested() && nowMs - lastRestartAtMs_ > 100) {
        lastRestartAtMs_ = nowMs;
        // Restart from any non-RUNNING state
        if (state_ == GameState::GAME_OVER || state_ == GameState::WIN || state_ == GameState::PAUSED) {
            initialize();
        }
    }

    // Debounce pause toggle: only process once per 100ms
    if (controls_.consumePauseToggleRequested() && nowMs - lastPauseToggleAtMs_ > 100) {
        lastPauseToggleAtMs_ = nowMs;
        if (state_ == GameState::RUNNING) {
            stateBeforePause_ = GameState::RUNNING;
            state_ = GameState::PAUSED;
        } else if (state_ == GameState::PAUSED) {
            state_ = stateBeforePause_;  // Resume to whatever state we were in
        }
    }
}

void GameLoop::onReshape(int width, int height) {
    renderer_.onReshape(width, height);
}

void GameLoop::applyDirectionInput() {
    Direction next;
    if (!controls_.consumePendingDirection(next)) {
        return;
    }

    if (state_ == GameState::START) {
        state_ = GameState::RUNNING;
    }

    if (state_ == GameState::RUNNING) {
        snake_.tryQueueDirection(next);
    }
}

void GameLoop::stepRunningState() {
    snake_.applyQueuedDirection();
    snake_.stepForward();

    if (!arena_.isInside(snake_.head()) || snake_.isSelfCollision()) {
        state_ = GameState::GAME_OVER;
        return;
    }

    if (snake_.head() == food_.position()) {
        snake_.growByOne();
        ++score_;

        // Check if we've crossed a speed milestone
        // Milestones: score 5 → 190ms, score 10 → 160ms, score 15 → 130ms, score 20 → 100ms
        int newSpeedTier = 0;
        if (score_ >= 20) newSpeedTier = 4;
        else if (score_ >= 15) newSpeedTier = 3;
        else if (score_ >= 10) newSpeedTier = 2;
        else if (score_ >= 5) newSpeedTier = 1;

        if (newSpeedTier > speedTier_) {
            speedTier_ = newSpeedTier;
            // Apply interval reduction: base 220ms - (speedTier * 30ms), capped at 100ms
            stepIntervalMs_ = GameConfig::kBaseStepMs - (speedTier_ * 30);
            if (stepIntervalMs_ < 100) {
                stepIntervalMs_ = 100;
            }
        }

        placeFood();
    }
}

void GameLoop::placeFood() {
    std::vector<GridPos> freeCells = arena_.collectFreeCells(snake_.segments());
    int seed = glutGet(GLUT_ELAPSED_TIME);
    seed ^= (score_ * 1103515245 + 12345);
    seed ^= (static_cast<int>(snake_.segments().size()) << 16);
    if (!food_.placeFromFreeCells(freeCells, seed)) {
        // No free cells left: board is full, player has won!
        state_ = GameState::WIN;
    }
}

}  // namespace snake3d
