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
      camera_(new Camera()) {
    glEnable(GL_DEPTH_TEST);
}

void Renderer::onReshape(int width, int height) {
    width_ = width;
    height_ = height;
    camera_->applyProjection(width_, height_);
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

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(static_cast<float>(arena_->width()), 0.0f, 0.0f);
    glVertex3f(static_cast<float>(arena_->width()), 0.0f, static_cast<float>(arena_->height()));
    glVertex3f(0.0f, 0.0f, static_cast<float>(arena_->height()));
    glEnd();
}

void Renderer::drawSnake() const {
    if (!snake_) {
        return;
    }

    const std::vector<GridPos>& segments = snake_->segments();
    for (size_t i = 0; i < segments.size(); ++i) {
        const GridPos& cell = segments[i];
        glPushMatrix();
        glTranslatef(static_cast<float>(cell.x) + 0.5f, 0.5f, static_cast<float>(cell.y) + 0.5f);

        if (i == 0) {
            glColor3f(0.2f, 0.9f, 0.3f);
        } else {
            glColor3f(0.1f, 0.6f, 0.2f);
        }

        glutSolidCube(0.92f);
        glPopMatrix();
    }
}

void Renderer::drawFood() const {
    if (!food_) {
        return;
    }

    const GridPos& f = food_->position();
    glPushMatrix();
    glTranslatef(static_cast<float>(f.x) + 0.5f, 0.45f, static_cast<float>(f.y) + 0.5f);
    glColor3f(0.9f, 0.2f, 0.2f);
    glutSolidSphere(0.35, 20, 20);
    glPopMatrix();
}

void Renderer::drawOverlay() const {
    char buffer[128];
    std::snprintf(buffer, sizeof(buffer), "Score: %d", score_);

    drawText(10.0f, static_cast<float>(height_ - 24), buffer);

    if (state_ == GameState::START) {
        drawText(10.0f, static_cast<float>(height_ - 48), "Press Arrow Key to Start");
    } else if (state_ == GameState::PAUSED) {
        drawText(10.0f, static_cast<float>(height_ - 48), "Paused (P to Resume)");
    } else if (state_ == GameState::GAME_OVER) {
        drawText(10.0f, static_cast<float>(height_ - 48), "Game Over (R to Restart)");
        std::snprintf(buffer, sizeof(buffer), "Final Score: %d", score_);
        drawText(10.0f, static_cast<float>(height_ - 72), buffer);
    } else if (state_ == GameState::WIN) {
        drawText(10.0f, static_cast<float>(height_ - 48), "You Won! Board Full (R to Restart)");
        std::snprintf(buffer, sizeof(buffer), "Final Score: %d", score_);
        drawText(10.0f, static_cast<float>(height_ - 72), buffer);
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

}  // namespace snake3d
