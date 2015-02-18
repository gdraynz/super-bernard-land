#ifndef __MUSIC_H_
#define __MUSIC_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Music
{
public:
	static Music* GetInstance(void);
	static void DestroyInstance(void);
	bool load_files(void);
	bool play_Music(void);
	bool play_jump(void);
	bool play_death(void);
	void stop_Music(void);

private:
	static Music* instance;
	//La musique qui sera jouée
	Mix_Music *music = NULL;
	//Les effets sonores que nous allons use
	Mix_Chunk *saut = NULL;
	Mix_Chunk *mort = NULL;
	Mix_Chunk *autre1 = NULL;
	Mix_Chunk *autre2 = NULL;

	Music();
	~Music() {}
};


#endif