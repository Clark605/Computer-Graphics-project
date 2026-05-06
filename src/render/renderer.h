#pragma once

#include <array>

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
    void drawBackdrop() const;
    void drawSurroundings() const;
    void drawPitWalls() const;
    void drawBoard() const;
    void drawSnake() const;
    void drawFood() const;
    void drawOverlay() const;
    void drawText(float x, float y, const char* text) const;
    const char* getStateMessage() const;
    void drawSpriteQuad(float centerX, float centerY, float centerZ, float size, unsigned int textureID) const;
    void drawSpriteQuad(float centerX, float centerY, float centerZ, float size, unsigned int textureID, bool flipV) const;
    
    void initializeTextures();
    unsigned int createGrassTexture(int size);
    unsigned int createPitTexture(int size);
    unsigned int createFallbackTexture(int size, float r, float g, float b);
    unsigned int loadTextureFromFile(const char* relativePath) const;
    unsigned int snakeTextureForSegment(size_t index) const;
    static Direction directionBetween(const GridPos& from, const GridPos& to);

    enum class SnakeTextureSlot {
        HEAD_UP,
        HEAD_RIGHT,
        HEAD_DOWN,
        HEAD_LEFT,
        TAIL_UP,
        TAIL_RIGHT,
        TAIL_DOWN,
        TAIL_LEFT,
        BODY_VERTICAL,
        BODY_HORIZONTAL,
        BODY_TOP_LEFT,
        BODY_TOP_RIGHT,
        BODY_BOTTOM_RIGHT,
        BODY_BOTTOM_LEFT,
        COUNT
    };

    SnakeTextureSlot snakeTextureSlotForSegment(size_t index) const;

    const Snake* snake_;
    const Food* food_;
    const Arena* arena_;
    GameState state_;
    int score_;
    int width_;
    int height_;
    Camera* camera_;
    bool texturesReady_;
    
    float boardYOffset_;
    unsigned int boardTextureID_;
    unsigned int grassTextureID_;
    unsigned int skyTextureID_;
    std::array<unsigned int, static_cast<size_t>(SnakeTextureSlot::COUNT)> snakeTextureIDs_;
    unsigned int foodTextureID_;
};

}  // namespace snake3d
