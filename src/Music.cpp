#include "Music.h"
#include "Logger.h"
#include <stdlib.h>

Music* Music::instance = new Music();

Music* Music::GetInstance(void)
{
	if (!instance)
		instance = new Music();
	return instance;
}

void Music::DestroyInstance(void)
{
	delete instance;
	instance = NULL;
}

/* Initialize the musique */
Music::Music()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}

bool Music::load_files(void){

	//Chargement de la musique
	music = Mix_LoadMUS("./sounds/BBT.wav");

	//S'il y a eu une erreur au chargement de la musique
	if (music == NULL)
	{
		return false;
	}
	logging("musique chargee");
	//Chargement des effets sonores
	saut = Mix_LoadWAV("./sounds/tongue.wav");
	mort = Mix_LoadWAV("./sounds/death.wav");
	autre1 = Mix_LoadWAV("./sounds/lol.wav");
	autre2 = Mix_LoadWAV("./sounds/lol.wav");
	//S'il y a eu un problÀme au chargement des effets sonore
	if ((saut == NULL) || (mort == NULL) || (autre1 == NULL) || (autre2 == NULL)){
		return false;
	}

	//Si tout s'est bien charg»
	return true;
}

bool Music::play_Music(void){
	if (Mix_PlayingMusic() == 0){
		//On lance la musique
		if (Mix_PlayMusic(music, -1) == -1){
			logging("ca joue la musiquelol");
			return true;
		}
	}
	return false;
}

bool Music::play_jump(void){
	if (Mix_PlayChannel(-1, saut, 0)){
		logging("ca joue la musiquelol");
		return true;
	}
	return false;
}

bool Music::play_death(void){
	if (Mix_PlayChannel(-1, mort, 0)){
		logging("ca joue la musiquelol");
		return true;
	}
	return false;
}

void Music::stop_Music(void){
	Mix_HaltMusic();
}
