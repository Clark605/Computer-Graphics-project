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
    ~Renderer();

    void onReshape(int width, int height);
    void setFrameState(GameState state, int score, const Snake& snake, const Food& food, const Arena& arena);
    void drawFrame() const;

private:
    void drawBoard() const;
    void drawSnake() const;
    void drawFood() const;
    void drawOverlay() const;
    void drawText(float x, float y, const char* text) const;
    const char* getStateMessage() const;
    
    void initializeTextures();
    unsigned int createCheckerboardTexture(int size);
    unsigned int createSolidTexture(int size, float r, float g, float b);

    const Snake* snake_;
    const Food* food_;
    const Arena* arena_;
    GameState state_;
    int score_;
    int width_;
    int height_;
    Camera* camera_;
    bool texturesReady_;
    
    unsigned int boardTextureID_;
    unsigned int snakeTextureID_;
    unsigned int foodTextureID_;
};

}  // namespace snake3d
