#include "engine.h"

typedef struct _camera {
    int xoffset, yoffset;
    int speed;
} camera;

void moveCam(camera *cam, mouse mouse_, int maxX, int maxY, double deltaTime) {
    if (mouse_.x >= maxX-20) {
        cam->xoffset -= cam->speed;
    } else if (mouse_.x <= 20) {
        cam->xoffset += cam->speed;
    }

    if (mouse_.y >= maxY-20) {
        cam->yoffset -= cam->speed;
    } else if (mouse_.y <= 20) {
        cam->yoffset += cam->speed;
    }
}
