#include "Monster.h"
#include "Logger.h"
#include "LevelGenerator.h"
#include "Application.h"

#define FRAMETIME 7
#define FRAMES 4

Monster::Monster()
{

}

/* init variable Monster */
Monster::Monster(int x, int y)
{
	hitbox = { x, y, MONSTERSIZEX, MONSTERSIZEY };
	walkUp = false;
	walkDown = false;
	walkLeft = true;
	walkRight = false;
	canJump = false;
	jump = false;
	speed = false;
	destRight = true;
	currentFrame = 0;
	isJumping = 0;
	canJumpAgain = false;
	isFalling = true;
	hPastWay = 0;
	space = 0;
	moveEnable = true;
	previousPastWay = 0;
	realSpeed = 1;
}

void Monster::Fall(void)
{
	if (moveEnable)
	{
		canJump = false;
		hitbox.y += 5;
	}
}

void Monster::canMoveUp(void)
{
	if (!APP->IsFloorType(LG->GetTile(APP->visionRect, space, hitbox.x + 5, hitbox.y - 1)) &&
		!APP->IsFloorType(LG->GetTile(APP->visionRect, space, hitbox.x + MONSTERSIZEX - 5, hitbox.y - 1))){
		//rien
	}
	else
		isFalling = true;
}

bool Monster::canMoveRight(void)
{
	return (!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + MONSTERSIZEX + 1, hitbox.y + MONSTERSIZEY - 1))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + MONSTERSIZEX + 1, hitbox.y + MONSTERSIZEY / 2))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + MONSTERSIZEX + 1, hitbox.y))));
}

bool Monster::canMoveLeft(void)
{
	return (!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y + MONSTERSIZEY - 1))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y + MONSTERSIZEY / 2))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y))));
}

void Monster::Move(void)
{
	if (moveEnable)
	{
		canMoveUp();

		if (!jump && canJump)
			canJumpAgain = true;
		else if (isFalling)
			canJumpAgain = false;

		if (canJump)
			isFalling = false;

		if (canJump && jump)
			isJumping = 0;

		if ((!canJump && !jump && isJumping > 0) || isJumping >= JUMPDURATION / 2)
			isFalling = true;

		if (isFalling){
			if (isJumping > 0)
				isJumping -= 2;
			hitbox.y -= 7 - ((JUMPDURATION - isJumping) / 5);
		}

		if (jump && !isFalling && canJumpAgain && (isJumping > 0 || !walkDown)){
			if (canJump || isJumping > 0){
				isJumping += 1;
				hitbox.y -= 11 - (isJumping / 5);
			}
			else
				canJumpAgain = false;
		}

		if (isJumping > 0 || isFalling || !walkDown){
			if (!(walkRight && walkLeft) && (walkRight || walkLeft))
			{	
				currentFrame += 1;

				if (walkRight && this->canMoveRight()){
					if (destRight)
						hitbox.x += realSpeed;
					destRight = true;
				}
				else {
					walkRight = false;
					walkLeft = true;
				}

				if (walkLeft && this->canMoveLeft()){
					if (!destRight)
						hitbox.x -= realSpeed;
					destRight = false;
				}
				else {
					walkRight = true;
					walkLeft = false;
				}
			}
			else
				currentFrame = -1;
		}

		if (previousPastWay != hPastWay){
			hitbox.x -= (hPastWay - previousPastWay);
			previousPastWay = hPastWay;
		}

		if (hitbox.x < 1 - hPastWay){
			walkRight = true;
			walkLeft = false;
		}			

		if (hitbox.x >= 620 + (TILESIZE * 100 - 640 - hPastWay)){
			walkRight = false;
			walkLeft = true;
		}		
	}
}

/* change sprite with Monster action */
void Monster::Draw(void)
{
	if (moveEnable){
		if (walkDown)
			APP->monst.DrawSpriteAt(hitbox, { 106, 11, 17, 15 }, destRight);
		else if (isFalling)
			APP->monst.DrawSpriteAt(hitbox, { 106, 11, 17, 15 }, destRight);
		else if (jump && !canJump)
			APP->monst.DrawSpriteAt(hitbox, { 106, 11, 17, 15 }, destRight);
		else if (!canJump)
			APP->monst.DrawSpriteAt(hitbox, { 106, 11, 17, 15 }, destRight);
		else
		{
			SDL_Rect src;
			if ((currentFrame / 7) % FRAMES == 1)
				src = { 7, 9, 19, 17 };
			else if ((currentFrame / 7) % FRAMES == 2)
				src = { 30, 9, 19, 17 };
			else if ((currentFrame / 7) % FRAMES == 3)
				src = { 55, 9, 19, 17 };
			else
				src = { 78, 9, 19, 17 };
			APP->monst.DrawSpriteAt(hitbox, src, destRight);
		}
	}
	else
	{
		// SPRITE DE QUAND IL MEURT LE CON
		APP->monst.DrawSpriteAt(hitbox, { 78, 9, 19, 17 }, destRight);
	}
}

int Monster::Die(void){

	moveEnable = false;

	return 0;
}
