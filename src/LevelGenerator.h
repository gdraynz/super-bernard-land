#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__

#include "SDL2/SDL_image.h"
#include <cstdlib>
#include <string>
#include "Monster.h"
#include "Utils.h"

using namespace std;

enum t_tile { EMPTY, BOX, FLOORTOP, FLOOR, FLOORLEFT, FLOORRIGHT, FLOORSIDELEFT, FLOORSIDERIGHT, CLOUD, TRAP, ENDFLAG, PLANT, PLANT2, PLANT3, WOOD };

class LevelGenerator
{
    public:
        static LevelGenerator* GetInstance(void);

        t_tile tileTypes[MAXVISIONX][MAXVISIONY];

        void InitLevel(void);
        t_tile GetTile(SDL_Rect visionRect, int space, int x, int y);
        SDL_Rect GetTileRect(SDL_Rect visionRect, int space, int x, int y);

    private:
        static LevelGenerator* instance;

        bool PixelIs(SDL_Surface* s, int x, int y, SDL_Color c);
        bool EqualsColor(SDL_Color sc1, SDL_Color sc2);
        Uint32 GetPixel(SDL_Surface* s, int x, int y);

        LevelGenerator() {}
        ~LevelGenerator() {}
};

#define LG LevelGenerator::GetInstance()

#endif