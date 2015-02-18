#include "LevelGenerator.h"
#include "Logger.h"
#include "Application.h"
#include "Options.h"


LevelGenerator* LevelGenerator::instance = NULL;

LevelGenerator* LevelGenerator::GetInstance(void)
{
    if (!instance)
        instance = new LevelGenerator();
    return instance;
}

bool LevelGenerator::PixelIs(SDL_Surface* s, int x, int y, SDL_Color c)
{
    Uint32 pix;
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    pix = this->GetPixel(s, x, y);
    SDL_GetRGB(pix, s->format, &red, &green, &blue);
    return EqualsColor({red, green, blue, 0}, c);
}

/* generate custom level with pixels color */
void LevelGenerator::InitLevel(void)
{
    int x, y;
    const string level = "images/map" + patch::to_string(OPTIONS->GetLevel()) + ".png";
    srand(time(NULL));

    SDL_Surface* s = IMG_Load(level.c_str());

    for (x = 0; x < MAXVISIONX; x++)
        for (y = 0; y < MAXVISIONY; y++)
        {
            if (PixelIs(s, x, y, FLOORCOLOR))
            {
                // Rien au dessus
                if (!PixelIs(s, x, y-1, FLOORCOLOR))
                {
                    if (PixelIs(s, x-1, y, FLOORCOLOR) && !PixelIs(s, x+1, y, FLOORCOLOR))
                        tileTypes[x][y] = FLOORRIGHT;
                    else if (!PixelIs(s, x-1, y, FLOORCOLOR) && PixelIs(s, x+1, y, FLOORCOLOR))
                        tileTypes[x][y] = FLOORLEFT;
                    else
                        tileTypes[x][y] = FLOORTOP;
                }
                else
                {
                    if (PixelIs(s, x-1, y, FLOORCOLOR) && !PixelIs(s, x+1, y, FLOORCOLOR))
                        tileTypes[x][y] = FLOORSIDERIGHT;
                    else if (!PixelIs(s, x-1, y, FLOORCOLOR) && PixelIs(s, x+1, y, FLOORCOLOR))
                        tileTypes[x][y] = FLOORSIDELEFT;
                    else
                        tileTypes[x][y] = FLOOR;
                }
            }
			else if (PixelIs(s, x, y, BLACK))
				tileTypes[x][y] = WOOD;
			else if (PixelIs(s, x, y, PLANTCOLOR))
                switch (rand()%3)
                {
                    case 0:
                        tileTypes[x][y] = PLANT;
                        break;
                    case 1:
                        tileTypes[x][y] = PLANT2;
                        break;
                    case 2:
                        tileTypes[x][y] = PLANT3;
                        break;
                }
			else if (PixelIs(s, x, y, COULEURCHELOU))
				tileTypes[x][y] = CLOUD;
			else if (PixelIs(s, x, y, GREEN))
				tileTypes[x][y] = TRAP;
			else if (PixelIs(s, x, y, YELLOW))
				tileTypes[x][y] = ENDFLAG;
            else if (PixelIs(s, x, y, MONSTERCOLOR))
                APP->monsters.push_back(new Monster(x * TILESIZE, y * TILESIZE));
            else
                tileTypes[x][y] = EMPTY;
        }

    SDL_FreeSurface(s);
    logging("Level initiated");
}

/* if color are equals */
bool LevelGenerator::EqualsColor(SDL_Color sc1, SDL_Color sc2)
{
    int r = sc1.r - sc2.r;
    int g = sc1.g - sc2.g;
    int b = sc1.b - sc2.b;
    if (r <= 5 && r >= -5)
        if (g <= 5 && g >= -5)
            if (b <= 5 && b >= -5)
                return true;
    return false;
}

// Get tile type at position x and y
t_tile LevelGenerator::GetTile(SDL_Rect visionRect, int space, int x, int y)
{
	return tileTypes[visionRect.x + ((x + space) / TILESIZE)][visionRect.y + (y / TILESIZE)];
}

// Get SDL_Rect of tile at position x and y
SDL_Rect LevelGenerator::GetTileRect(SDL_Rect visionRect, int space, int x, int y)
{
    return { ((visionRect.x+((x + space)/TILESIZE))-visionRect.x)*TILESIZE, ((visionRect.y+(y/TILESIZE))-visionRect.y)*TILESIZE, TILESIZE, TILESIZE };
}

/* return pixel */
Uint32 LevelGenerator::GetPixel(SDL_Surface* s, int x, int y)
{
    int bpp = s->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)s->pixels + y * s->pitch + x * bpp;

    switch(bpp)
    {
    case 1:
        return *p;
        break;
    case 2:
        return *(Uint16 *)p;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;
    case 4:
        return *(Uint32 *)p;
        break;
    default:
        return 0;
    }
}
