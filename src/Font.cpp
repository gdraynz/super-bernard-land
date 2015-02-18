#include "Font.h"
#include "Logger.h"
#include "Utils.h"

#include <iostream>


/* Put text in upper */
std::string Font::ToUpper(const std::string& s)
{
	std::string result;

	std::locale loc;
	for (unsigned int i = 0; i < s.length(); ++i)
	{
		result += std::toupper(s.at(i), loc);
	}

	return result;
}

/* Put text in lower */
std::string Font::ToLower(const std::string& s)
{
	std::string result;

	std::locale loc;
	for (unsigned int i = 0; i < s.length(); ++i)
	{
		result += std::tolower(s.at(i), loc);
	}

	return result;
}

/* Set the texture */
void Font::RenderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/* Set the Text */
SDL_Texture* Font::RenderText(const std::string &message, SDL_Color color,
                              int fontSize, SDL_Renderer *renderer)
{
	font = TTF_OpenFont(this->fontPath.c_str(), fontSize);
	if (!font){
		logging(TTF_GetError());
		return NULL;
	}
	surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (!surf){
		logging(TTF_GetError());
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (!texture){
		logging(TTF_GetError());
		return NULL;
	}

	TTF_CloseFont(font);
	font = NULL;
	SDL_FreeSurface(surf);
	return texture;
}

/* Set the font */
Font::Font(const std::string &fontPath, SDL_Renderer *ren)
{
	tex = NULL;
	surf = NULL;
	font = NULL;
	this->fontPath = fontPath;
	this->ren = ren;

	/* valeur par défaut */
	this->x = 0;
	this->y = 0;
	this->isCenter = false;
	this->isShadow = false;
	this->shadowColor = { 0, 0, 0, 0 };
	this->color = { 0, 0, 0, 0 };
	this->size = 20;
	this->isContour = false;
}

/* Draw the text from the given message */
bool Font::DrawText(const std::string &message)
{
	if (this->isShadow)
		PartText(ToUpper(message), this->x + size / 4, this->y + size / 4, this->shadowColor);

	PartText(ToUpper(message), this->x, this->y, this->color);

	if (this->isContour)
		PartText(ToLower(message), this->x, this->y, { 0, 0, 0, 0 });

	return true;
}

/* Draw each part of the text */
bool Font::PartText(const std::string &message, int x, int y, SDL_Color color)
{
	int corx;
	int cory;

	/* TTF gestion */
	tex = RenderText(message.c_str(), color, size, ren);
	if (tex == NULL){
		return false;
	}

	//Get the texture w/h so we can center it in the screen
	if (isCenter)
	{
		int iW, iH;
		SDL_QueryTexture(tex, NULL, NULL, &iW, &iH);
		corx = x - iW / 2;
		cory = y - iH / 2;
	}
	else
	{
		corx = x;
		cory = y;
	}

	/* Dessiner le texte */
	RenderTexture(tex, ren, corx, cory);

	SDL_DestroyTexture(tex);

	return true;
}

/* Set text style */
void Font::SetStyle(bool isShadow, bool isContour, int size)
{
	this->SetShadow(isShadow);
	this->SetContour(isContour);
	this->SetSize(size);
}

void Font::SetShadow(bool isShadow)
{
	if (isShadow)
		this->isShadow = isShadow;
}

void Font::SetContour(bool isContour)
{
	if (isContour)
		this->isContour = isContour;
}

void Font::SetSize(int size)
{
	if (size)
		this->size = size;
}

/* Set text position */
void Font::SetPos(int x, int y, bool isCenter)
{
	this->SetCoord(x, y);
	this->SetCenter(isCenter);
}

/* Set the coordinates */
void Font::SetCoord(int x, int y)
{
	if (x && y)
	{
		this->x = x;
		this->y = y;
	}
}

void Font::SetCenter(bool isCenter)
{
	if (isCenter)
		this->isCenter = isCenter;
}

/* Set text color */
void Font::SetColors(SDL_Color color, SDL_Color shadowColor)
{
	this->SetColor(color);
	this->SetShadowColor(shadowColor);
}

void Font::SetColor(SDL_Color color)
{
	this->color = color;
}

void Font::SetShadowColor(SDL_Color shadowColor)
{
	this->shadowColor = shadowColor;
}

/* Set all details for text */
void Font::SetFont(const std::string &fontPath)
{
	if (fontPath != "")
		this->fontPath = fontPath;
}
