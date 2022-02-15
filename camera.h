#pragma once

typedef struct _camera {
    int xoffset, yoffset;
    int speed;
} camera;

void moveCam(camera *cam, mouse mouse_, int maxX, int maxY, double deltaTime);
