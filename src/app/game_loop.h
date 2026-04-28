#pragma once

#include <vector>

namespace snake3d {

enum class GameState {
    START,
    RUNNING,
    PAUSED,
    GAME_OVER,
    WIN
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct GridPos {
    int x;
    int y;

    bool operator==(const GridPos& other) const {
        return x == other.x && y == other.y;
    }
};

struct GameConfig {
    static constexpr int kArenaWidth = 12;
    static constexpr int kArenaHeight = 12;
    static constexpr int kWindowWidth = 600;
    static constexpr int kWindowHeight = 400;
    static constexpr int kBaseStepMs = 220;
};

class Arena;
class Snake;
class Food;
class Controls;
class Renderer;

class GameLoop {
public:
    GameLoop(Arena& arena, Snake& snake, Food& food, Controls& controls, Renderer& renderer);

    void initialize();
    void update(int nowMs);
    void render() const;

    void onSpecialKey(int key);
    void onKeyboard(unsigned char key);
    void onReshape(int width, int height);

private:
    void applyDirectionInput();
    void stepRunningState();
    void placeFood();

    Arena& arena_;
    Snake& snake_;
    Food& food_;
    Controls& controls_;
    Renderer& renderer_;

    GameState state_;
    GameState stateBeforePause_;  // Track state to restore after unpause
    int score_;
    int speedTier_;
    int stepIntervalMs_;
    int lastStepAtMs_;
    int lastPauseToggleAtMs_;     // Debounce rapid pause inputs
    int lastRestartAtMs_;         // Debounce rapid restart inputs
};

}  // namespace snake3d
