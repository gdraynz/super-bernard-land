#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

class Image
{
	public:
		Image() {}
        Image(const std::string &imgPath);
		Image(const std::string &imgPath, int h, int w);
		~Image() {}

		void DrawSpriteAt(SDL_Rect dest, SDL_Rect src, bool flip);
		void Draw(void);
		void DrawAt(int x, int y);
        void Close(void);

	private:
        int h;
        int w;
        string imgPath;

		SDL_Texture *tex;

        void CheckTexture(void);
};


#endif