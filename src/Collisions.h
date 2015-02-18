#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include <SDL2/SDL.h>


class Collisions
{
    public:
    	static bool CollisionFlour(SDL_Rect box, int a);
    	static bool CollisionBoxes(SDL_Rect box1, SDL_Rect box2);

    private:
        Collisions();
        ~Collisions() {}
};

#endif