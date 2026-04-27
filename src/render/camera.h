#pragma once

namespace snake3d {

class Camera {
public:
    Camera();

    void applyProjection(int width, int height) const;
    void applyView() const;
};

}  // namespace snake3d
