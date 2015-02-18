#include "Collisions.h"


/* Detect a collision with the flour */
bool Collisions::CollisionFlour(SDL_Rect box, int a)
{
   if (box.y + box.h >= a)
      return true;
   else
      return false;
}

/* Detect a collision between 2 boxes */
bool Collisions::CollisionBoxes(SDL_Rect box1, SDL_Rect box2)
{
   if((box2.x >= box1.x + box1.w) || (box2.x + box2.w <= box1.x)
	|| (box2.y >= box1.y + box1.h) || (box2.y + box2.h <= box1.y))
        return false;
   else
        return true;
}