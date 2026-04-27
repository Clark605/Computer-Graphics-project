#pragma once

#include "app/game_loop.h"

namespace snake3d {

class Arena;
class Food;
class Snake;
class Camera;

class Renderer {
public:
    Renderer();

    void onReshape(int width, int height);
    void setFrameState(GameState state, int score, const Snake& snake, const Food& food, const Arena& arena);
    void drawFrame() const;

private:
    void drawBoard() const;
    void drawSnake() const;
    void drawFood() const;
    void drawOverlay() const;
    void drawText(float x, float y, const char* text) const;

    const Snake* snake_;
    const Food* food_;
    const Arena* arena_;
    GameState state_;
    int score_;
    int width_;
    int height_;
    Camera* camera_;
};

}  // namespace snake3d
