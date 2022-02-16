#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "engine.h"
#include "mapgen.h"
#include "camera.h"


//=========================================================================== GAME VARIABLES =================================================================//


#define win_width 1920
#define win_height 1080

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
camera mainCamera = {.speed = 3, .xoffset = 0, .yoffset = 0};
mouse mouse_;

const unsigned int timePerFrame = 1000 / 240;
const float deltaTimeRatio = 1000 / 500;

bool run = true;

//========================================================================== INIT ============================================================================//


void init() {
    SDL_Init( SDL_INIT_VIDEO );
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow( "Try", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_width, win_height, SDL_WINDOW_FULLSCREEN_DESKTOP );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}


//========================================================================== CLOSE ===========================================================================//


void Close_() {
    SDL_DestroyWindow( window );

    SDL_DestroyRenderer( renderer);

    SDL_DestroyTexture(map_texture);

    SDL_Quit();
    IMG_Quit();
}

//=========================================================================== EVENT HANDLER ==================================================================//


void eventHandler(SDL_Event event) {
    while (SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_QUIT:
                run = false;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        run = false;
                        break;
                }

            case SDL_MOUSEMOTION:
                mouse_.x = event.motion.x;
                mouse_.y = event.motion.y;

            default:
                break;
        }
    }
}

//=========================================================================== GAME RENDERER ===================================================================//


void gameRenderer() {

    SDL_RenderClear(renderer);
    drawMap(renderer, mainCamera);
    SDL_RenderPresent(renderer);
}


//=========================================================================== GAME ===========================================================================//


void runGame() {
    init();
    loadMap(renderer);

    SDL_Event event;

    Uint32 frameStart;
    Uint32 frameEnd;
    Uint32 frameTime;

    float deltaTime;


    while (run) {
        frameStart = SDL_GetTicks();

        // Game

        gameRenderer();
        eventHandler(event);
        moveCam(&mainCamera, mouse_, win_width, win_height, deltaTime);


        // Game

        frameEnd = SDL_GetTicks();
        frameTime = frameEnd - frameStart;

        if (frameTime < timePerFrame) {
            SDL_Delay(timePerFrame - frameTime);
            frameTime = timePerFrame;
        }

        deltaTime = (float)(frameTime / deltaTimeRatio);
    }

    Close_();
}
