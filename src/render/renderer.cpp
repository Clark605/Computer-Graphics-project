/*
 * Renderer: Board, Snake, Food, and UI Overlay Drawing
 *
 * CURRENT IMPLEMENTATION: Procedural textures (classroom-friendly, no external assets)
 *   - Board: Checkerboard texture on the grid surface
 *   - Snake: Solid green texture with brighter head
 *   - Food: Solid red texture (glutSolidSphere)
 *   - Overlays: Text via GLUT bitmap font in 2D orthographic mode
 *
 * TEXTURE BINDING PATTERN (for future enhancement):
 *   1. Load texture files from assets/textures/ at renderer initialization
 *   2. Create OpenGL texture objects: glGenTextures(1, &textureID)
 *   3. Bind texture: glBindTexture(GL_TEXTURE_2D, textureID)
 *   4. Set parameters: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
 *   5. Load image data: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData)
 *   6. Enable texturing in draw calls: glEnable(GL_TEXTURE_2D) before drawing, glDisable after
 *   7. Cleanup: glDeleteTextures(1, &textureID) in destructor
 *
 * BINDING LIFECYCLE:
 *   - Initialize after GL context is created (done in onReshape)
 *   - Bind in each draw call where texture is needed (glBindTexture before drawing)
 *   - Unbind after use (optional; rebinding overwrites)
 *   - Delete in destructor before context is lost
 *
 * For classroom use, the current solid-color approach is recommended for clarity.
 */

#include "render/renderer.h"

#include <cstdio>

#include <gl/glut.h>

#include "gameplay/arena.h"
#include "gameplay/food.h"
#include "gameplay/snake.h"
#include "render/camera.h"

namespace snake3d {

Renderer::Renderer()
    : snake_(nullptr),
      food_(nullptr),
      arena_(nullptr),
      state_(GameState::START),
      score_(0),
      width_(900),
      height_(700),
      camera_(new Camera()),
            texturesReady_(false),
      boardTextureID_(0),
      snakeTextureID_(0),
      foodTextureID_(0) {
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
    if (boardTextureID_ != 0) glDeleteTextures(1, &boardTextureID_);
    if (snakeTextureID_ != 0) glDeleteTextures(1, &snakeTextureID_);
    if (foodTextureID_ != 0) glDeleteTextures(1, &foodTextureID_);
    delete camera_;
}

void Renderer::onReshape(int width, int height) {
    width_ = width;
    height_ = height;
    camera_->applyProjection(width_, height_);
    if (!texturesReady_) {
        initializeTextures();
        texturesReady_ = true;
    }
}

void Renderer::setFrameState(GameState state, int score, const Snake& snake, const Food& food, const Arena& arena) {
    state_ = state;
    score_ = score;
    snake_ = &snake;
    food_ = &food;
    arena_ = &arena;
}

void Renderer::drawFrame() const {
    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_->applyView();

    drawBoard();
    drawSnake();
    drawFood();
    drawOverlay();

    glutSwapBuffers();
}

void Renderer::drawBoard() const {
    if (!arena_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boardTextureID_);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    float w = static_cast<float>(arena_->width());
    float h = static_cast<float>(arena_->height());
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(w, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(w, 0.0f, h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, h);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawSnake() const {
    if (!snake_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, snakeTextureID_);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    
    const std::vector<GridPos>& segments = snake_->segments();
    for (size_t i = 0; i < segments.size(); ++i) {
        const GridPos& cell = segments[i];
        glPushMatrix();
        glTranslatef(static_cast<float>(cell.x) + 0.5f, 0.5f, static_cast<float>(cell.y) + 0.5f);

        if (i == 0) {
            glColor3f(0.3f, 1.0f, 0.4f);  // Brighter green for head
        } else {
            glColor3f(0.2f, 0.8f, 0.3f);  // Slightly darker for body
        }

        glutSolidCube(0.92f);
        glPopMatrix();
    }
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawFood() const {
    if (!food_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, foodTextureID_);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    
    const GridPos& f = food_->position();
    glPushMatrix();
    glTranslatef(static_cast<float>(f.x) + 0.5f, 0.45f, static_cast<float>(f.y) + 0.5f);
    glColor3f(1.0f, 0.3f, 0.3f);  // Brighter red
    glutSolidSphere(0.35, 20, 20);
    glPopMatrix();
    
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawOverlay() const {
    char buffer[128];
    std::snprintf(buffer, sizeof(buffer), "Score: %d", score_);
    drawText(10.0f, static_cast<float>(height_ - 24), buffer);

    const char* stateMsg = getStateMessage();
    if (stateMsg) {
        drawText(10.0f, static_cast<float>(height_ - 48), stateMsg);
    }

    if (state_ == GameState::GAME_OVER || state_ == GameState::WIN) {
        std::snprintf(buffer, sizeof(buffer), "Final Score: %d", score_);
        drawText(10.0f, static_cast<float>(height_ - 72), buffer);
    }
}

const char* Renderer::getStateMessage() const {
    switch (state_) {
        case GameState::START:
            return "Press Arrow Key to Start";
        case GameState::PAUSED:
            return "Paused (P to Resume)";
        case GameState::GAME_OVER:
            return "Game Over (R to Restart)";
        case GameState::WIN:
            return "You Won! Board Full (R to Restart)";
        default:
            return nullptr;  // RUNNING state: no state message
    }
}

void Renderer::drawText(float x, float y, const char* text) const {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<double>(width_), 0.0, static_cast<double>(height_));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);
    for (const char* p = text; *p != '\0'; ++p) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *p);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::initializeTextures() {
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    boardTextureID_ = createCheckerboardTexture(GameConfig::kArenaWidth);
    snakeTextureID_ = createSolidTexture(32, 0.2f, 0.9f, 0.3f);  // Green
    foodTextureID_ = createSolidTexture(32, 0.9f, 0.2f, 0.2f);   // Red
}

unsigned int Renderer::createCheckerboardTexture(int size) {
    unsigned char* data = new unsigned char[size * size * 3];
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int idx = (i * size + j) * 3;
            // One texel per grid cell for perfect alignment
            bool isWhite = ((i + j) % 2) == 0;
            unsigned char color = isWhite ? 220 : 150;
            data[idx] = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
        }
    }
    
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    delete[] data;
    return texID;
}

unsigned int Renderer::createSolidTexture(int size, float r, float g, float b) {
    unsigned char* data = new unsigned char[size * size * 3];
    
    unsigned char rc = static_cast<unsigned char>(r * 255);
    unsigned char gc = static_cast<unsigned char>(g * 255);
    unsigned char bc = static_cast<unsigned char>(b * 255);
    
    for (int i = 0; i < size * size; ++i) {
        data[i * 3] = rc;
        data[i * 3 + 1] = gc;
        data[i * 3 + 2] = bc;
    }
    
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] data;
    return texID;
}

}  // namespace snake3d
