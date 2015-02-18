#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <list>
#include "Player.h"
#include "Menu.h"
#include "Monster.h"
#include "Music.h"
#include "LevelGenerator.h"

#include <stdlib.h>

using namespace std;

class Application
{
    public:
        static Application* GetInstance(void);
        static void DestroyInstance(void);

        bool isStarted;
        SDL_Rect visionRect;
        Image sprite;
		Image monst;
        Monster monster;
        int cpDeath;
		bool IsFloorType(t_tile tile);
        list<Monster*> monsters;

		SDL_Renderer* GetRenderer(void);
		void Update(void);
        bool InitWindow(void);
		void ClearRender(void);
        void Close(void);
        void Start(void);
		void Stop(void);
        void ShowMenu(void);

        // Close everything
        void EndApplication(void);

        void ShowToolTip(string str, SDL_Color c);

    private:
        static Application* instance;

        Image background;
        Image box;
        Image floor;
		Image cloud;
        Image trap;
		Image endFlag;
		Image plant;
		Image wood;

        SDL_Window* appWindow;
        SDL_Renderer* appRender;
		SDL_Event appEvent;
        Player* player;
		Music* daMusic;

        int HandleKeyDown(void);
        int HandleKeyUp(void);
		int MainLoop(void);
		bool CheckCollision(int x, int y, int sizex, int sizey);
        void CheckDeath(int x, int y);
        void CheckFinish(int x);
		void CheckDeathByMonster(SDL_Rect p, SDL_Rect m);
		bool changeDifficult;

		int currentEvent = 0;
		int elapsedTime = 0;
		int beginTime = 0;
        int startedTime = 0;

        Application();
        ~Application() {}
};

// Application call
#define APP Application::GetInstance()

#endif