#include "render/camera.h"

#include <gl/glut.h>

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
    gluLookAt(10.0, 22.0, 28.0, 10.0, 0.0, 10.0, 0.0, 1.0, 0.0);
}

}  // namespace snake3d
