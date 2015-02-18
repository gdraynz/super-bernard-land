#include "Player.h"
#include "Logger.h"
#include "LevelGenerator.h"
#include "Application.h"

#define FRAMETIME 7
#define FRAMES 4

Player* Player::instance = NULL;

Player* Player::GetInstance(void)
{
    if (!instance)
        instance = new Player();
    return instance;
}

void Player::DestroyInstance(void)
{
	delete instance;
	instance = NULL;
}

/* init variable player */
Player::Player()
{
    hitbox       = { 0, 195, PLAYERSIZEX, PLAYERSIZEY };
    walkUp       = false;
    walkDown     = false;
    walkLeft     = false;
    walkRight    = false;
	canJump      = false;
	jump         = false;
	speed        = false;
    destRight    = true;
	currentFrame = 0;
	isJumping    = 0;
	canJumpAgain = false;
    isFalling    = true;
	hPastWay     = 0;
	space        = 0;
	moveEnable   = true;
	sounds		 = Music::GetInstance();
}

void Player::Fall(void)
{
	if (moveEnable)
	{
		canJump = false;
		hitbox.y += 5;
	}
}

void Player::canMoveUp(void)
{
	if (!APP->IsFloorType(LG->GetTile(APP->visionRect, space, hitbox.x + 5, hitbox.y - 1)) &&
		!APP->IsFloorType(LG->GetTile(APP->visionRect, space, hitbox.x + PLAYERSIZEX - 5, hitbox.y - 1))){
		//rien
	}
	else
		isFalling = true;
}

bool Player::canMoveRight(void)
{
	return (!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + PLAYERSIZEX + 1, hitbox.y + PLAYERSIZEY - 1))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + PLAYERSIZEX + 1, hitbox.y + PLAYERSIZEY / 2))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x + PLAYERSIZEX + 1, hitbox.y))));
}

bool Player::canMoveLeft(void)
{
	return (!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y + PLAYERSIZEY - 1))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y + PLAYERSIZEY / 2))) &&
		!APP->IsFloorType((LG->GetTile(APP->visionRect, space, hitbox.x - 1, hitbox.y))));
}

void Player::Move(void)
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
			if (isJumping == 0)
				sounds->play_jump();
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
				int realSpeed = 0;
				if (speed){
					currentFrame += 2;
					realSpeed = 4;
				}
				else{
					currentFrame += 1;
					realSpeed = 2;
				}

				if (walkRight && this->canMoveRight()){
					if (destRight)
						hitbox.x += realSpeed;
					destRight = true;
				}
				if (walkLeft && this->canMoveLeft()){
					if (!destRight)
						hitbox.x -= realSpeed;
					destRight = false;
				}
			}
			else
				currentFrame = -1;
		}

		if (hitbox.x < 1)
			hitbox.x = 1;

		if (hitbox.x >= 620)
			hitbox.x = 620;

		if (hitbox.x < HMIDDLESCREEN - CENTER && hPastWay > 0){
			hPastWay -= HMIDDLESCREEN - CENTER - hitbox.x;
			hitbox.x = HMIDDLESCREEN - CENTER;
		}

		if (hitbox.x > HMIDDLESCREEN + CENTER && hPastWay < TILESIZE * 100 - 641){
			hPastWay += hitbox.x - HMIDDLESCREEN - CENTER;
			hitbox.x = HMIDDLESCREEN + CENTER;
		}
	}

}

/* change sprite with player action */
void Player::Draw(void)
{
	if (moveEnable){
		if (walkDown)
			APP->sprite.DrawSpriteAt(hitbox, { 245, 168, 26, 31 }, !destRight);
		else if (isFalling)
			APP->sprite.DrawSpriteAt(hitbox, { 234, 47, 26, 31 }, !destRight);
		else if (jump && !canJump)
			APP->sprite.DrawSpriteAt(hitbox, { 267, 5, 26, 31 }, !destRight);
		else if (!canJump)
			APP->sprite.DrawSpriteAt(hitbox, { 234, 47, 26, 31 }, !destRight);
		else
		{
			SDL_Rect src;
			if (currentFrame >= 0){
				if ((currentFrame / FRAMETIME) % FRAMES == 0)
					src = { 52, 5, 26, 31 };
				else if ((currentFrame / FRAMETIME) % FRAMES == 1)
					src = { 83, 5, 26, 31 };
				else if ((currentFrame / FRAMETIME) % FRAMES == 2)
					src = { 114, 5, 26, 31 };
				else if ((currentFrame / FRAMETIME) % FRAMES == 3)
					src = { 145, 5, 26, 31 };
			}
			else
				src = { 27, 5, 25, 31 };

			APP->sprite.DrawSpriteAt(hitbox, src, !destRight);
		}
	}
	else
	{
		// SPRITE DE QUAND IL MEURT LE CON
		APP->sprite.DrawSpriteAt(hitbox, { 25, 43, 26, 31 }, !destRight);
	}
}

int Player::Die(void)
{
	if (moveEnable)
    {
		APP->cpDeath++;
		sounds->play_death();
		deathTime = SDL_GetTicks();
		logging("temps : " + patch::to_string(deathTime));
	}

	APP->ShowToolTip("YOU DIED " + patch::to_string(APP->cpDeath) + " TIMES", RED);
	moveEnable = false;

	elapsedTime = SDL_GetTicks() - deathTime;
	if (elapsedTime > 750)
    	return RESTARTGAME;

	return CONTINUE;
}
