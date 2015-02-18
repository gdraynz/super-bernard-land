#include "Image.h"
#include "Logger.h"
#include "Application.h"

Image::Image(const std::string &imgPath)
{
	this->h = 0;
	this->w = 0;
	this->tex = NULL;
	this->imgPath = imgPath;
}

Image::Image(const std::string &imgPath, int h, int w)
{
	this->h = h;
	this->w = w;
	this->tex = NULL;
	this->imgPath = imgPath;
}

/* Draw the image by creating a texture */
void Image::Draw(void)
{
	CheckTexture();

	SDL_Renderer* ren = APP->GetRenderer();

	/* Dessiner la texture */
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, this->tex, NULL, NULL);
}

void Image::DrawAt(int x, int y)
{
	CheckTexture();

	SDL_Renderer* ren = APP->GetRenderer();

	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	if (this->h == 0 || this->w == 0)
		SDL_QueryTexture(this->tex, NULL, NULL, &dst.w, &dst.h);
	else
	{
		dst.w = this->w;
		dst.h = this->h;
	}

	/* Dessiner la texture */
	SDL_RenderCopy(ren, this->tex, NULL, &dst);
}

void Image::CheckTexture(void)
{
	if (!this->tex)
	{
		/* Chargement de l'image */
		SDL_Surface *img = IMG_Load(imgPath.c_str());
		/* Conversion de la SDL_surface en SDL_texture */
		this->tex = SDL_CreateTextureFromSurface(APP->GetRenderer(), img);
		/* Clean */
		SDL_FreeSurface(img);
	}
}

void Image::DrawSpriteAt(SDL_Rect dest, SDL_Rect src, bool flip)
{
	CheckTexture();

	SDL_Renderer* ren = APP->GetRenderer();

	if (flip)
		SDL_RenderCopyEx(ren, tex, &src, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
	else
		SDL_RenderCopyEx(ren, tex, &src, &dest, 0, NULL, SDL_FLIP_NONE);
}

void Image::Close(void)
{
	if (tex)
		SDL_DestroyTexture(this->tex);
}