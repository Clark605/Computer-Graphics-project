#include "render/camera.h"

#include <gl/glut.h>

#include "app/game_loop.h"

namespace snake3d {

Camera::Camera() = default;

void Camera::applyProjection(int width, int height) const {
    int safeHeight = height > 0 ? height : 1;
    glViewport(0, 0, width, safeHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, static_cast<double>(width) / static_cast<double>(safeHeight), 1.0, 200.0);
}

void Camera::applyView() const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    const float sizeX = static_cast<float>(GameConfig::kArenaWidth);
    const float sizeZ = static_cast<float>(GameConfig::kArenaHeight);
    const float centerX = sizeX * 0.5f;
    const float centerZ = sizeZ * 0.5f;

    // Center the camera on the board and move closer for a tighter view.
    const float eyeX = centerX;
    const float eyeY = sizeX * 1.4f;
    const float eyeZ = sizeZ * 1.6f;

    gluLookAt(eyeX, eyeY, eyeZ, centerX, 0.0f, centerZ, 0.0f, 1.0f, 0.0f);
}

}  // namespace snake3d
