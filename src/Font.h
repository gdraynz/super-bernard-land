#ifndef __FONT_H_
#define __FONT_H_

#include <SDL2/SDL_ttf.h>
#include <string>
#include <locale>

using namespace std;

class Font
{
	public:
		Font() {}
		Font(const std::string &fontPath, SDL_Renderer *ren);
		~Font() {}

		bool DrawText(const std::string &message);

		void SetStyle(bool isShadow, bool isContour, int size);
		void SetFont(const std::string &fontPath);
		void SetShadow(bool isShadow);
		void SetContour(bool isContour);
		void SetSize(int size);

		void SetPos(int x, int y, bool isCenter);
		void SetCoord(int x, int y);
		void SetCenter(bool isCenter);

		void SetColors(SDL_Color color, SDL_Color shadowColor);
		void SetShadowColor(SDL_Color shadowColor);
		void SetColor(SDL_Color color);

	private:
		SDL_Surface *surf;
		SDL_Texture *tex;
		SDL_Renderer *ren;
		TTF_Font *font;

		std::string fontPath;
		int x;
		int y;

		bool isShadow;
		bool isCenter;
		bool isContour;
		SDL_Color shadowColor;
		SDL_Color color;
		int size;

		std::string ToUpper(const std::string &s);
		std::string ToLower(const std::string &s);
		bool PartText(const std::string &message, int x, int y, SDL_Color color);
		void RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
		SDL_Texture* RenderText(const std::string &message, SDL_Color color,
		                        int fontSize, SDL_Renderer *renderer);
};


#endif