/*
 * Renderer: Board, Snake, Food, and UI Overlay Drawing
 *
 * ASSET-BACKED IMPLEMENTATION:
 *   - Grass: moss2.png around the board
 *   - Backdrop: skyhorizon01.png behind the scene
 *   - Snake: direction-specific PNG sprites for head, body, and tail
 *   - Food: apple.png sprite
 *   - Board: procedural brown pit texture
 *   - Overlays: Text via GLUT bitmap font in 2D orthographic mode
 *
 * If a texture fails to load, the renderer falls back to a procedural placeholder.
 */

#include "render/renderer.h"

#define NOMINMAX

#include <windows.h>
#include <wincodec.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <gl/glut.h>

#include "gameplay/arena.h"
#include "gameplay/food.h"
#include "gameplay/snake.h"
#include "render/camera.h"

namespace snake3d {

namespace {

struct ComScope {
    bool initialized;

    ComScope() : initialized(false) {
        HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        initialized = SUCCEEDED(result) || result == S_FALSE;
    }

    ~ComScope() {
        if (initialized) {
            CoUninitialize();
        }
    }
};

struct TreePlacement {
    float x;
    float z;
    float scale;
    int variant;
};

std::wstring widenPath(const char* path) {
    return std::wstring(path, path + std::strlen(path));
}

GLuint uploadTextureFromPixels(UINT width, UINT height, const std::vector<unsigned char>& pixels, GLenum wrapS, GLenum wrapT) {
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    return textureID;
}

GLuint loadPngTexture(const char* path, GLenum wrapS, GLenum wrapT) {
    ComScope com;
    IWICImagingFactory* factory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;
    GLuint textureID = 0;
    UINT width = 0;
    UINT height = 0;

    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)))) {
        return 0;
    }

    if (FAILED(factory->CreateDecoderFromFilename(widenPath(path).c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder))) {
        factory->Release();
        return 0;
    }

    if (FAILED(decoder->GetFrame(0, &frame)) || FAILED(frame->GetSize(&width, &height))) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (factory) factory->Release();
        return 0;
    }

    if (FAILED(factory->CreateFormatConverter(&converter)) ||
        FAILED(converter->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut))) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (factory) factory->Release();
        return 0;
    }

    std::vector<unsigned char> topDownPixels(static_cast<size_t>(width) * static_cast<size_t>(height) * 4u);
    if (FAILED(converter->CopyPixels(nullptr, width * 4u, static_cast<UINT>(topDownPixels.size()), topDownPixels.data()))) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (factory) factory->Release();
        return 0;
    }

    std::vector<unsigned char> flippedPixels(topDownPixels.size());
    const size_t rowBytes = static_cast<size_t>(width) * 4u;
    for (UINT y = 0; y < height; ++y) {
        std::memcpy(&flippedPixels[(height - 1u - y) * rowBytes], &topDownPixels[y * rowBytes], rowBytes);
    }

    textureID = uploadTextureFromPixels(width, height, flippedPixels, wrapS, wrapT);

    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    if (factory) factory->Release();
    return textureID;
}

void drawTreeModel(float x, float z, float scale, int variant) {
    const float trunkColors[][3] = {
        {0.36f, 0.24f, 0.12f},
        {0.40f, 0.26f, 0.14f},
        {0.33f, 0.22f, 0.11f}
    };

    const float leafColors[][3] = {
        {0.12f, 0.38f, 0.14f},
        {0.14f, 0.43f, 0.16f},
        {0.16f, 0.35f, 0.10f}
    };

    const int paletteIndex = variant % 3;

    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);

    glColor3f(trunkColors[paletteIndex][0], trunkColors[paletteIndex][1], trunkColors[paletteIndex][2]);
    glPushMatrix();
    glTranslatef(0.0f, 0.28f, 0.0f);
    glScalef(0.22f, 0.56f, 0.22f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glColor3f(leafColors[paletteIndex][0], leafColors[paletteIndex][1], leafColors[paletteIndex][2]);
    glPushMatrix();
    glTranslatef(0.0f, 0.95f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.78f, 1.15f, 6, 2);
    glPopMatrix();

    glColor3f(leafColors[paletteIndex][0] * 0.88f, leafColors[paletteIndex][1] * 0.88f, leafColors[paletteIndex][2] * 0.88f);
    glPushMatrix();
    glTranslatef(0.0f, 1.48f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.54f, 0.88f, 6, 2);
    glPopMatrix();

    glPopMatrix();
}

}  // namespace

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
      boardYOffset_(-0.22f),
      boardTextureID_(0),
      grassTextureID_(0),
      skyTextureID_(0),
      foodTextureID_(0) {
    snakeTextureIDs_.fill(0);
    glEnable(GL_DEPTH_TEST);
}

Renderer::~Renderer() {
    if (boardTextureID_ != 0) glDeleteTextures(1, &boardTextureID_);
    if (grassTextureID_ != 0) glDeleteTextures(1, &grassTextureID_);
    if (skyTextureID_ != 0) glDeleteTextures(1, &skyTextureID_);
    for (unsigned int textureID : snakeTextureIDs_) {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
    }
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
    glClearColor(0.04f, 0.08f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera_->applyView();

    drawBackdrop();
    drawSurroundings();
    drawTrees();
    drawPitWalls();
    drawBoard();
    drawFood();
    drawSnake();
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
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, boardYOffset_, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(w, boardYOffset_, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(w, boardYOffset_, h);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, boardYOffset_, h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawBackdrop() const {
    if (!arena_ || skyTextureID_ == 0) {
        return;
    }

    const float boardWidth = static_cast<float>(arena_->width());
    const float boardHeight = static_cast<float>(arena_->height());

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyTextureID_);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.0f, 0.0f, boardHeight + 6.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(boardWidth + 4.0f, 0.0f, boardHeight + 6.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(boardWidth + 4.0f, 11.0f, boardHeight + 6.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.0f, 11.0f, boardHeight + 6.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawSurroundings() const {
    if (!arena_ || grassTextureID_ == 0) {
        return;
    }

    const float boardWidth = static_cast<float>(arena_->width());
    const float boardHeight = static_cast<float>(arena_->height());
    // Increase margin so grass covers the full frame edges at larger aspect ratios
    const float margin = 6.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTextureID_);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    // Bottom strip
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-margin, 0.0f, -margin);
    glTexCoord2f(5.0f, 1.0f); glVertex3f(boardWidth + margin, 0.0f, -margin);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(boardWidth + margin, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-margin, 0.0f, 0.0f);

    // Top strip
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-margin, 0.0f, boardHeight);
    glTexCoord2f(5.0f, 1.0f); glVertex3f(boardWidth + margin, 0.0f, boardHeight);
    glTexCoord2f(5.0f, 0.0f); glVertex3f(boardWidth + margin, 0.0f, boardHeight + margin);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-margin, 0.0f, boardHeight + margin);

    // Left strip
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-margin, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.0f, boardHeight);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-margin, 0.0f, boardHeight);

    // Right strip
    glTexCoord2f(0.0f, 1.0f); glVertex3f(boardWidth, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(boardWidth + margin, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(boardWidth + margin, 0.0f, boardHeight);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(boardWidth, 0.0f, boardHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawTrees() const {
    if (!arena_) {
        return;
    }

    const float boardWidth = static_cast<float>(arena_->width());
    const float boardHeight = static_cast<float>(arena_->height());

    const TreePlacement placements[] = {
        {-3.0f, -2.0f, 0.95f, 0},
        {boardWidth + 2.3f, -1.4f, 1.05f, 1},
        {-2.7f, boardHeight + 2.5f, 0.90f, 2},
        {boardWidth + 2.0f, boardHeight + 2.2f, 1.10f, 0},
        {boardWidth * 0.18f, -3.0f, 0.82f, 1},
        {boardWidth * 0.82f, boardHeight + 3.0f, 0.88f, 2},
    };

    glDisable(GL_TEXTURE_2D);
    for (const TreePlacement& placement : placements) {
        drawTreeModel(placement.x, placement.z, placement.scale, placement.variant);
    }
}

void Renderer::drawPitWalls() const {
    if (!arena_) {
        return;
    }

    const float boardWidth = static_cast<float>(arena_->width());
    const float boardHeight = static_cast<float>(arena_->height());
    const float wallTop = 0.0f;
    const float wallBottom = boardYOffset_;
    const float inset = 0.02f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boardTextureID_);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-inset, wallBottom, -inset);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(boardWidth + inset, wallBottom, -inset);
    glTexCoord2f(4.0f, 1.0f); glVertex3f(boardWidth + inset, wallTop, -inset);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-inset, wallTop, -inset);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-inset, wallBottom, boardHeight + inset);
    glTexCoord2f(4.0f, 0.0f); glVertex3f(boardWidth + inset, wallBottom, boardHeight + inset);
    glTexCoord2f(4.0f, 1.0f); glVertex3f(boardWidth + inset, wallTop, boardHeight + inset);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-inset, wallTop, boardHeight + inset);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-inset, wallBottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, wallBottom, 0.0f);
    glTexCoord2f(1.0f, 4.0f); glVertex3f(0.0f, wallTop, boardHeight);
    glTexCoord2f(0.0f, 4.0f); glVertex3f(-inset, wallTop, boardHeight);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(boardWidth, wallBottom, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(boardWidth + inset, wallBottom, 0.0f);
    glTexCoord2f(1.0f, 4.0f); glVertex3f(boardWidth + inset, wallTop, boardHeight);
    glTexCoord2f(0.0f, 4.0f); glVertex3f(boardWidth, wallTop, boardHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawSnake() const {
    if (!snake_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const std::vector<GridPos>& segments = snake_->segments();
    for (size_t i = 0; i < segments.size(); ++i) {
        const GridPos& cell = segments[i];
        // Determine texture slot so we can adjust V flipping for corner sprites
        SnakeTextureSlot slot = snakeTextureSlotForSegment(i);
        unsigned int tex = snakeTextureIDs_[static_cast<size_t>(slot)];
        bool flipV = true;
        switch (slot) {
            case SnakeTextureSlot::BODY_TOP_LEFT:
                flipV = false;
                break;
            case SnakeTextureSlot::BODY_TOP_RIGHT:
            case SnakeTextureSlot::BODY_BOTTOM_LEFT:
                // These corner sprites appear inverted with the original V orientation.
                flipV = false;
                break;
            case SnakeTextureSlot::BODY_BOTTOM_RIGHT:
                flipV = false;
                break;
            default:
                flipV = true;
                break;
        }
        drawSpriteQuad(static_cast<float>(cell.x) + 0.5f, boardYOffset_ + 0.03f, static_cast<float>(cell.y) + 0.5f, 0.92f, tex, flipV);
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawFood() const {
    if (!food_) {
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GridPos& f = food_->position();
    drawSpriteQuad(static_cast<float>(f.x) + 0.5f, boardYOffset_ + 0.03f, static_cast<float>(f.y) + 0.5f, 0.82f, foodTextureID_);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Renderer::drawSpriteQuad(float centerX, float centerY, float centerZ, float size, unsigned int textureID) const {
    drawSpriteQuad(centerX, centerY, centerZ, size, textureID, true);
}

void Renderer::drawSpriteQuad(float centerX, float centerY, float centerZ, float size, unsigned int textureID, bool flipV) const {
    const float half = size * 0.5f;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    if (flipV) {
        // Flipped V coordinates
        glTexCoord2f(0.0f, 1.0f); glVertex3f(centerX - half, centerY, centerZ - half);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(centerX + half, centerY, centerZ - half);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(centerX + half, centerY, centerZ + half);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(centerX - half, centerY, centerZ + half);
    } else {
        // Original V coordinates
        glTexCoord2f(0.0f, 0.0f); glVertex3f(centerX - half, centerY, centerZ - half);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(centerX + half, centerY, centerZ - half);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(centerX + half, centerY, centerZ + half);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(centerX - half, centerY, centerZ + half);
    }
    glEnd();
}

Direction Renderer::directionBetween(const GridPos& from, const GridPos& to) {
    if (to.x > from.x) return Direction::RIGHT;
    if (to.x < from.x) return Direction::LEFT;
    if (to.y > from.y) return Direction::DOWN;
    return Direction::UP;
}

unsigned int Renderer::snakeTextureForSegment(size_t index) const {
    if (!snake_) {
        return 0;
    }

    if (snake_->segments().empty()) {
        return 0;
    }
    SnakeTextureSlot slot = snakeTextureSlotForSegment(index);
    return snakeTextureIDs_[static_cast<size_t>(slot)];
}

Renderer::SnakeTextureSlot Renderer::snakeTextureSlotForSegment(size_t index) const {
    if (!snake_) {
        return SnakeTextureSlot::BODY_BOTTOM_LEFT;
    }

    const std::vector<GridPos>& segments = snake_->segments();
    if (segments.empty()) {
        return SnakeTextureSlot::BODY_BOTTOM_LEFT;
    }

    if (index == 0) {
        switch (snake_->direction()) {
            case Direction::UP: return SnakeTextureSlot::HEAD_UP;
            case Direction::RIGHT: return SnakeTextureSlot::HEAD_RIGHT;
            case Direction::DOWN: return SnakeTextureSlot::HEAD_DOWN;
            case Direction::LEFT: return SnakeTextureSlot::HEAD_LEFT;
        }
    }

    if (index + 1 == segments.size()) {
        Direction tailDirection = directionBetween(segments[index - 1], segments[index]);
        switch (tailDirection) {
            case Direction::UP: return SnakeTextureSlot::TAIL_UP;
            case Direction::RIGHT: return SnakeTextureSlot::TAIL_RIGHT;
            case Direction::DOWN: return SnakeTextureSlot::TAIL_DOWN;
            case Direction::LEFT: return SnakeTextureSlot::TAIL_LEFT;
        }
    }

    Direction fromPrev = directionBetween(segments[index - 1], segments[index]);
    Direction toNext = directionBetween(segments[index], segments[index + 1]);

    const bool vertical = (fromPrev == Direction::UP || fromPrev == Direction::DOWN) &&
                          (toNext == Direction::UP || toNext == Direction::DOWN);
    if (vertical) {
        return SnakeTextureSlot::BODY_VERTICAL;
    }

    const bool horizontal = (fromPrev == Direction::LEFT || fromPrev == Direction::RIGHT) &&
                            (toNext == Direction::LEFT || toNext == Direction::RIGHT);
    if (horizontal) {
        return SnakeTextureSlot::BODY_HORIZONTAL;
    }

    if ((fromPrev == Direction::LEFT && toNext == Direction::UP) ||
        (fromPrev == Direction::DOWN && toNext == Direction::RIGHT)) {
        return SnakeTextureSlot::BODY_TOP_LEFT;
    }
    if ((fromPrev == Direction::UP && toNext == Direction::RIGHT) ||
        (fromPrev == Direction::LEFT && toNext == Direction::DOWN)) {
        return SnakeTextureSlot::BODY_TOP_RIGHT;
    }
    if ((fromPrev == Direction::RIGHT && toNext == Direction::DOWN) ||
        (fromPrev == Direction::UP && toNext == Direction::LEFT)) {
        return SnakeTextureSlot::BODY_BOTTOM_RIGHT;
    }
    if ((fromPrev == Direction::RIGHT && toNext == Direction::UP) ||
        (fromPrev == Direction::DOWN && toNext == Direction::LEFT)) {
        return SnakeTextureSlot::BODY_BOTTOM_LEFT;
    }

    return SnakeTextureSlot::BODY_BOTTOM_LEFT;
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
            return nullptr;
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

    grassTextureID_ = loadTextureFromFile("assets/textures/moss2.png");
    if (grassTextureID_ == 0) {
        grassTextureID_ = createGrassTexture(64);
    }

    skyTextureID_ = loadTextureFromFile("assets/textures/skyhorizon01.png");
    if (skyTextureID_ == 0) {
        skyTextureID_ = createFallbackTexture(64, 0.46f, 0.64f, 0.88f);
    }

    boardTextureID_ = createPitTexture(64);

    const char* snakePaths[] = {
        "assets/textures/head_up.png",
        "assets/textures/head_right.png",
        "assets/textures/head_down.png",
        "assets/textures/head_left.png",
        "assets/textures/tail_up.png",
        "assets/textures/tail_right.png",
        "assets/textures/tail_down.png",
        "assets/textures/tail_left.png",
        "assets/textures/body_vertical.png",
        "assets/textures/body_horizontal.png",
        "assets/textures/body_topleft.png",
        "assets/textures/body_topright.png",
        "assets/textures/body_bottomright.png",
        "assets/textures/body_bottomleft.png"
    };

    for (size_t i = 0; i < snakeTextureIDs_.size(); ++i) {
        snakeTextureIDs_[i] = loadTextureFromFile(snakePaths[i]);
        if (snakeTextureIDs_[i] == 0) {
            snakeTextureIDs_[i] = createFallbackTexture(32, 0.24f, 0.82f, 0.36f);
        }
    }

    foodTextureID_ = loadTextureFromFile("assets/textures/apple.png");
    if (foodTextureID_ == 0) {
        foodTextureID_ = createFallbackTexture(32, 0.96f, 0.36f, 0.14f);
    }
}

unsigned int Renderer::createGrassTexture(int size) {
    unsigned char* data = new unsigned char[size * size * 3];

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int idx = (y * size + x) * 3;
            bool blade = ((y * 3 + x * 5) % 11) < 3;
            bool shade = ((x / 4 + y / 4) % 2) == 0;
            unsigned char r = blade ? 30 : (shade ? 42 : 34);
            unsigned char g = blade ? 116 : (shade ? 132 : 118);
            unsigned char b = blade ? 30 : (shade ? 42 : 34);
            data[idx] = r;
            data[idx + 1] = g;
            data[idx + 2] = b;
        }
    }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;
    return texID;
}

unsigned int Renderer::createPitTexture(int size) {
    unsigned char* data = new unsigned char[size * size * 3];

    const float center = (static_cast<float>(size) - 1.0f) * 0.5f;
    const float radius = center * 0.95f;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int idx = (y * size + x) * 3;
            float dx = static_cast<float>(x) - center;
            float dy = static_cast<float>(y) - center;
            float dist = std::sqrt(dx * dx + dy * dy) / radius;
            float shade = dist > 1.0f ? 1.0f : dist;
            float soil = 0.45f + 0.55f * shade;
            float speck = static_cast<float>((x * 17 + y * 13) % 23) / 23.0f;
            data[idx] = static_cast<unsigned char>((58.0f + speck * 24.0f) * soil);
            data[idx + 1] = static_cast<unsigned char>((34.0f + speck * 16.0f) * soil);
            data[idx + 2] = static_cast<unsigned char>((18.0f + speck * 10.0f) * soil);
        }
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

unsigned int Renderer::createFallbackTexture(int size, float r, float g, float b) {
    unsigned char* data = new unsigned char[size * size * 3];

    unsigned char rc = static_cast<unsigned char>(r * 255.0f);
    unsigned char gc = static_cast<unsigned char>(g * 255.0f);
    unsigned char bc = static_cast<unsigned char>(b * 255.0f);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    delete[] data;
    return texID;
}

unsigned int Renderer::loadTextureFromFile(const char* relativePath) const {
    if (std::strcmp(relativePath, "assets/textures/moss2.png") == 0) {
        return loadPngTexture(relativePath, GL_REPEAT, GL_REPEAT);
    }

    return loadPngTexture(relativePath, GL_CLAMP, GL_CLAMP);
}

}  // namespace snake3d