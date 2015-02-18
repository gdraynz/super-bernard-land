#ifndef __MENU_H__
#define __MENU_H__

#include <list>
#include "Image.h"
#include "Font.h"
#include "Utils.h"

using namespace std;


class Menu
{
    public:
        static Menu* GetInstance(void);

        int onButton;

        void Show(void);
        bool CatchEvent(void);
        void PressStartMenu(void);
        void Close(void);

    private:
        static Menu* instance;

        list<string> buttons;
        int onMenu;

        Image background;

		bool ok = true;
		int actualTime = 0;
		int previousTime = 0;
		int beginTime = 0;

        void NextButton(void);
        void PrecButton(void);
        void CreateButton(string fontPath, string text,
                          int posx, int posy, int size,
                          SDL_Color c, SDL_Color shadow);

        void ShowOptionsMenu(void);
        void PrintBackground(void);
        void PrintMainMenu(void);
        void Update(void);

        Menu(const std::string &imgPath);
        ~Menu() {}
};

// Menu call
#define MENU Menu::GetInstance()

#endif