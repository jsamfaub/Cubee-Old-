#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "rect.h"
#include <iostream>
using namespace std;

extern SDL_Renderer* mRenderer;

rect::rect()
{
	posx = posy = w = h = 0;
	dest = {0,0,0,0};
	filled = 0;
	col = { 0,0,0,0 };
}

void rect::render(int x, int y)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	SDL_SetRenderDrawColor(mRenderer, col.r, col.g, col.b, col.a);
	if (filled)
	{
		SDL_RenderFillRect(mRenderer, &dest);
	}
	else
	{
		SDL_RenderDrawRect(mRenderer, &dest);
	}

	dest.x = posx;
	dest.y = posy;
}

int rect::getx()
{
	return posx;
}
int rect::gety()
{
	return posy;
}
int rect::getw()
{
	return w;
}
int rect::geth()
{
	return h;
}

void rect::setx(int x)
{
	posx = x;
}
void rect::sety(int y)
{
	posy = y;
}
void rect::setw(int width)
{
	w = width;
}
void rect::seth(int height)
{
	h = height;
}

bool rect::isFilled()
{
	return filled;
}

void rect::fill()
{
	filled = 1;
}
void rect::empty()
{
	filled = 0;
}

void rect::setCol(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	col.r = r;
	col.g = g;
	col.b = b;
	col.a = a;
}

SDL_Rect& rect::getCollider()
{
	return dest;
}
