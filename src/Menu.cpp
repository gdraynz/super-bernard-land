#include "Menu.h"
#include "Logger.h"
#include "Application.h"
#include "Options.h"


Menu* Menu::instance = NULL;

Menu* Menu::GetInstance(void)
{
    if (!instance)
        instance = new Menu("./images/titlescreen.bmp");
    return instance;
}

/*init background menu*/
Menu::Menu(const std::string &imgPath)
{
    background = Image(imgPath);
}

/* Display elements */
void Menu::Show(void)
{
    this->onButton = 1;
    this->onMenu = 1;
    while (!this->buttons.empty())
        this->buttons.pop_front();
    if (APP->isStarted)
        this->buttons.push_back("Resume");
    else
        this->buttons.push_back("Play");
    this->buttons.push_back("Options");
    this->buttons.push_back("Quit");
    this->Update();
    logging("Main menu shown, listening events");
    this->CatchEvent();
}

/* press start menu */
void Menu::PressStartMenu(void)
{
	SDL_Event appEvent;
	beginTime = SDL_GetTicks();
    int b = CONTINUE;

    while (b == CONTINUE)
    {
    	while (SDL_PollEvent(&appEvent)){
    		switch (appEvent.type){
    			case SDL_QUIT:
    				APP->EndApplication();
    			case SDL_KEYDOWN:
    				switch (appEvent.key.keysym.sym)
                    {
    					case SDLK_RETURN:
    						b = STARTGAME;
                            break;
    					case SDLK_ESCAPE:
                            APP->EndApplication();
    					default:
    						break;
    				}
    				break;
    			default:
    				break;
    		}
    	}

    	this->PrintBackground();

    	if (ok)
    		CreateButton(MAINFONT, "Press Enter", 320, 300, 22, MENUBLUE, SHADOWBLUE);

    	actualTime = SDL_GetTicks();
    	if (actualTime - previousTime >= 300)
    	{
    		previousTime = actualTime;
            ok = !ok;
    	}

    	int elapsedTime = SDL_GetTicks() - beginTime;
    	if (elapsedTime < 16)
    		SDL_Delay(16 - elapsedTime);
    	APP->Update();
    }
}

/* catch any event */
bool Menu::CatchEvent(void)
{
    SDL_Event menuEvent;

    while (1)
    {
        while (SDL_WaitEvent(&menuEvent))
        {
            switch (menuEvent.type)
            {
            case SDL_QUIT:
                APP->EndApplication();
            case SDL_KEYDOWN:
                switch (menuEvent.key.keysym.sym)
                {
                case SDLK_UP:
                    this->PrecButton();
                    break;
                case SDLK_DOWN:
                    this->NextButton();
                    break;
                case SDLK_ESCAPE:
                    APP->EndApplication();
                case SDLK_SPACE:
                case SDLK_RETURN:
                    if (onMenu == 1)
                    {
                        switch (this->onButton)
                        {
                            case 1:
                                logging("Game started from main menu");
                                APP->Start();
                                break;
                            case 2:
                                this->ShowOptionsMenu();
                                break;
                            case 3:
                                APP->EndApplication();
                            default :
                                break;
                        }
                    }
                    else if (onMenu == 2)
                    {
                        switch (this->onButton)
                        {
                            case 1:
                                logging("Easy");
                                OPTIONS->SetDifficulty(EASY);
                                this->Update();
                                break;
                            case 2:
                                logging("Normal");
                                OPTIONS->SetDifficulty(NORMAL);
                                this->Update();
                                break;
                            case 3:
                                logging("Hard");
                                OPTIONS->SetDifficulty(HARD);
                                this->Update();
                                break;
                            case 4:
								this->Show();
                                break;

                            default :
                                break;
                        }
                    }
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }
	return true;
}

// PRIVATE

void Menu::ShowOptionsMenu(void)
{
    this->onButton = 1;
    this->onMenu = 2;
    while (!this->buttons.empty())
        this->buttons.pop_front();
    this->buttons.push_back(STR_EASY);
    this->buttons.push_back(STR_NORMAL);
    this->buttons.push_back(STR_HARD);
    this->buttons.push_back("Return");
    this->Update();
}

void Menu::PrintBackground(void)
{
    background.Draw();
    CreateButton(MAINFONT, "Super", 320, 80, 70, RED, SHADOWBLACK);
    CreateButton(MAINFONT, "Bernard Land", 320, 165, 70, RED, SHADOWBLACK);
}

void Menu::PrintMainMenu(void)
{
    int x;
    SDL_Color color;
    list<string>::iterator i;
    for (x=0, i = this->buttons.begin(); i != this->buttons.end(); ++i, x++)
    {
        string t = *i;
        color = MENUBLUE;
        if (onButton == x+1)
            color = WHITE;
        else if (onMenu == 2)
            if (x == OPTIONS->GetDifficulty())
                color = YELLOW;
        CreateButton(MAINFONT, t, 320, 280 + (45*x), 40,
                     color, SHADOWBLUE);
    }
}

/* Update the menu background and buttons */
void Menu::Update(void)
{
    this->PrintBackground();
    this->PrintMainMenu();
    APP->Update();
}

/* Buttons gestion */
void Menu::PrecButton(void)
{
    this->onButton--;
    if (this->onButton <= 0)
        this->onButton = this->buttons.size();
    this->Update();
}

void Menu::NextButton(void)
{
    this->onButton++;
    if ((unsigned) this->onButton > this->buttons.size())
        this->onButton = 1;
    this->Update();
}

/* add a button */
void Menu::CreateButton(string fontPath, string t,
                        int posx, int posy, int size,
                        SDL_Color c, SDL_Color shadow)
{
    Font font = Font(fontPath.c_str(), APP->GetRenderer());
    font.SetStyle(true, true, size);
    font.SetPos(posx, posy, true);
    font.SetColors(c, shadow);
    font.DrawText(t);
}

void Menu::Close(void)
{
    this->background.Close();
}
