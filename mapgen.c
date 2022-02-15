#include "engine.h"
#include "camera.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#define mSize 30

typedef struct block_ {
    enum type {
        TL = 1,
        TM,
        TR,
        ML,
        MM,
        MR,
        BL,
        BM,
        BR,
        CTL,
        CTR,
        CBL,
        CBR
    } type;
    int x, y;

} block;


typedef struct _room {
    int width, height, x, y;
} room;

int map[mSize][mSize];
block bMap[mSize][mSize];

SDL_Texture* map_texture = NULL;

void createRoom(room room_) {
    for (int i = 0; i < room_.height; i++)
    {
        for (int j = 0; j < room_.width; j++) {
            map[i + room_.y][j + room_.x] = 1;
            bMap[i + room_.y][j + room_.x].x = j + room_.x;
            bMap[i + room_.y][j + room_.x].y = i + room_.y;
        }
    }
}

void connectRoom(room srcRoom, room dstRoom) {
    room con;

    if (srcRoom.y + srcRoom.height > dstRoom.y && srcRoom.y < dstRoom.y + dstRoom.height) {
        con.width = (dstRoom.x - (srcRoom.x + srcRoom.width));
        con.height = 3;
        con.x = srcRoom.x + srcRoom.width;
        con.y = srcRoom.y + ((int)((srcRoom.y + srcRoom.height) - dstRoom.y - 3 )/2);
    } else if (srcRoom.x + srcRoom.width > dstRoom.x && srcRoom.x < dstRoom.x + dstRoom.width) {
        con.width = 3;
        con.height = (dstRoom.y - (srcRoom.y + srcRoom.height));
        con.y = srcRoom.y + srcRoom.height;
        con.x = srcRoom.x + ((int)((srcRoom.x + srcRoom.width) - dstRoom.x - 3 )/2);
    }

    createRoom(con);
}


void generateMap() {

}

void getBlockType() {
    int type;
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            if (map[i][j] != NULL) {
                type = 0b000000;
                if (map[i+1][j] == NULL) {
                    type = (type | 0b0001);
                }
                if (map[i-1][j] == NULL) {
                    type = (type | 0b0010);
                }
                if (map[i][j+1] == NULL) {
                    type = (type | 0b0100);
                }
                if (map[i][j-1] == NULL) {
                    type = (type | 0b1000);
                }

                switch (type) {
                    case 0b0000:
                        if (map[i-1][j-1] == NULL) {
                            bMap[i][j].type = CTR;
                        }
                        else if (map[i-1][j+1] == NULL) {
                            bMap[i][j].type = CTL;
                        }
                        else if (map[i+1][j-1] == NULL) {
                            bMap[i][j].type = CBR;
                        }
                        else if (map[i+1][j+1] == NULL) {
                            bMap[i][j].type = CBL;
                        }
                        else {
                            bMap[i][j].type = MM;
                        }
                        break;
                    case 0b0001:
                        bMap[i][j].type = BM;
                        break;
                    case 0b0010:
                        bMap[i][j].type = TM;
                        break;
                    case 0b0100:
                        bMap[i][j].type = MR;
                        break;
                    case 0b1000:
                        bMap[i][j].type = ML;
                        break;
                    case 0b1001:
                        bMap[i][j].type = BL;
                        break;
                    case 0b0110:
                        bMap[i][j].type = TR;
                        break;
                    case 0b1010:
                        bMap[i][j].type = TL;
                        break;
                    case 0b0101:
                        bMap[i][j].type = BR;
                        break;
                }
            }
        }
    }
}

void loadMap(SDL_Renderer *renderer){
    srand(time(NULL));

    room rm0 = {5, 5, 10, 10};
    room rm1 = {8, 5, 20, 10};
    room rm2 = {6, 5, 10, 17};
    createRoom(rm0);
    createRoom(rm1);
    createRoom(rm2);
    connectRoom(rm0, rm1);
    connectRoom(rm0, rm2);

    getBlockType();

    map_texture = loadTexture("ground.png", renderer);
}

void drawMap(SDL_Renderer *renderer, camera cam){
    bool render = false;
    int type;
    SDL_Rect srcR = {0, 0, 64, 64};
    SDL_Rect dstR = {0, 0, 64, 64};
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            render = true;
            type = bMap[i][j].type;
            switch (type) {
                case TL:
                    srcR.x = 0;
                    srcR.y = 0;
                    break;
                case TM:
                    srcR.x = 64;
                    srcR.y = 0;
                    break;
                case TR:
                    srcR.x = 128;
                    srcR.y = 0;
                    break;
                case ML:
                    srcR.x = 0;
                    srcR.y = 64;
                    break;
                case MM:
                    srcR.x = 64;
                    srcR.y = 64;
                    break;
                case MR:
                    srcR.x = 128;
                    srcR.y = 64;
                    break;
                case BL:
                    srcR.x = 0;
                    srcR.y = 128;
                    break;
                case BM:
                    srcR.x = 64;
                    srcR.y = 128;
                    break;
                case BR:
                    srcR.x = 128;
                    srcR.y = 128;
                    break;
                case CTR:
                    srcR.x = 192;
                    srcR.y = 64;
                    break;
                case CTL:
                    srcR.x = 192;
                    srcR.y = 0;
                    break;
                case CBR:
                    srcR.x = 192;
                    srcR.y = 128;
                    break;
                case CBL:
                    srcR.x = 192;
                    srcR.y = 192;
                    break;
                default:
                    render = false;
                    break;
            }

        if (render) {
            dstR.x = j*dstR.h + (1920-mSize*64)/2 + cam.xoffset;
            dstR.y = i*dstR.w + (1080-mSize*64)/2 + cam.yoffset;
            SDL_RenderCopy(renderer, map_texture, &srcR, &dstR);
        }

        }
    }
}
