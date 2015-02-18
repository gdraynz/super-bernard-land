#include <stdexcept>

#include "Logger.h"
#include "Application.h"
#include "Image.h"
#include "Font.h"
#include "Utils.h"
#include "Menu.h"


int main(int argc, char** argv)
{
	for (int x = 0; x < argc - 1; x++){
		logging(argv[argc]);
	}

	/* Initialisation simple */
    if (SDL_Init(SDL_INIT_VIDEO) != 0 ){
		logging(SDL_GetError());
        return -1;
    }

	/* Initialisation de l'image */
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		logging("Img_init failed");
	}

	/* Initialisation de la font */
	if (TTF_Init() == -1){
		logging(TTF_GetError());
	}

    atexit(SDL_Quit);

    APP->InitWindow();
    APP->ShowMenu();
    APP->EndApplication();

	return (0);
}
