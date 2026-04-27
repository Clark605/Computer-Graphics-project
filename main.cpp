#include <gl/glut.h>

#include <cstdlib>

#include "app/game_loop.h"
#include "gameplay/arena.h"
#include "gameplay/food.h"
#include "gameplay/snake.h"
#include "input/controls.h"
#include "render/renderer.h"

namespace {

snake3d::Arena g_arena(snake3d::GameConfig::kArenaWidth, snake3d::GameConfig::kArenaHeight);
snake3d::Snake g_snake;
snake3d::Food g_food;
snake3d::Controls g_controls;
snake3d::Renderer g_renderer;
snake3d::GameLoop g_loop(g_arena, g_snake, g_food, g_controls, g_renderer);

void Display() {
    g_loop.render();
}

void Idle() {
    g_loop.update(glutGet(GLUT_ELAPSED_TIME));
    glutPostRedisplay();
}

void Reshape(int width, int height) {
    g_loop.onReshape(width, height);
}

void SpecialKey(int key, int, int) {
    g_loop.onSpecialKey(key);
}

void Keyboard(unsigned char key, int, int) {
    if (key == 27) {
        std::exit(0);
        return;
    }

    g_loop.onKeyboard(key);
}

}  // namespace

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(snake3d::GameConfig::kWindowWidth, snake3d::GameConfig::kWindowHeight);
    glutCreateWindow("Snake 3D");

    g_loop.initialize();

    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutReshapeFunc(Reshape);
    glutSpecialFunc(SpecialKey);
    glutKeyboardFunc(Keyboard);

    glutMainLoop();
    return 0;
}