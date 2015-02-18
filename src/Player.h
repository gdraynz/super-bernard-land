#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <string>
#include "Image.h"
#include "Font.h"
#include "EntityInterface.h"
#include "Music.h"

#define JUMPDURATION 40
#define PLAYERSIZEX 25
#define PLAYERSIZEY 40
#define HMIDDLESCREEN 310
#define CENTER 40

class Player: public IEntity
{
    public:
        static Player* GetInstance(void);
        static void DestroyInstance(void);

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

    private:
        static Player* instance;

		int currentFrame;
		int deathTime;
		int elapsedTime;
		int isJumping;
		bool isFalling;
		bool canJumpAgain;

        bool canMoveLeft(void);
        bool canMoveRight(void);
        void canMoveUp(void);

		Music *sounds;

        Player();
        virtual ~Player() {}
};

#endif
