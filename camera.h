#pragma once
#include <SDL2/SDL.h>

SDL_Texture* map_texture;

typedef struct _camera {
    float xoffset, yoffset;
    int speed;
} camera;

void moveCam(camera *cam, mouse mouse_, int maxX, int maxY, float deltaTime);
