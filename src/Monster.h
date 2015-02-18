#ifndef __MONSTER_H__
#define __MONSTER_H__

#include <string>
#include "Image.h"
#include "Font.h"
#include "EntityInterface.h"

#define JUMPDURATION 40
#define MONSTERSIZEX 20
#define MONSTERSIZEY 20
#define HMIDDLESCREEN 310
#define CENTER 40

class Monster: public IEntity
{
public:
	Monster(int x, int y);
	Monster();
	~Monster() {}

	bool walkUp;
	bool walkDown;
	bool walkLeft;
	bool walkRight;
	bool jump;
	bool canJump;
	bool speed;
	bool destRight;
	bool moveEnable;

	int space;
	int hPastWay;
	int cpDeath;
	SDL_Rect hitbox;

	void Draw(void);
	void Fall(void);
	void Move(void);
	int Die(void);
	int previousPastWay;
	int realSpeed;

private:
	int currentFrame;
	int isJumping;
	bool isFalling;
	bool canJumpAgain;

	bool canMoveLeft(void);
	bool canMoveRight(void);
	void canMoveUp(void);
};

#endif