#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "background.h"
#include <iostream>
using namespace std;

extern SDL_Renderer* mRenderer;

background::background(int sw, int sh)
{
	w = sw;
	h = sh;
	posx = 0;
	posy = 0;
	texture = NULL;
	dest = { 0,0,0,0 };
}

background::~background()
{
	close();
}

bool background::loadTexture(string path)
{
	close();
	bool success = 1;

	SDL_Surface* surface = NULL;
	surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		cout << "Couldn't load " << path << "! Error: " << IMG_GetError() << endl;
		success = 0;
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (texture == NULL)
		{
			cout << "Couldn't create texture from surface! Error: " << SDL_GetError() << endl;
			success = 0;
		}

		SDL_FreeSurface(surface);
	}

	return success;
}

void background::render(SDL_Rect camera)
{
	while (camera.x > w)
		camera.x -= w;
	while (camera.y > h)
		camera.y -= h;
	dest.x = -camera.x;
	dest.y = -camera.y;
	dest.w = w;
	dest.h = h;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	dest.x += w;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	dest.y += h;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	dest.x -= w;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	/*
	dest.x -= w;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	dest.y -= h;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	dest.x += w;
	SDL_RenderCopy(mRenderer, texture, NULL, &dest);
	*/
}

void background::close()
{
	if (texture != NULL)
	{
		SDL_DestroyTexture(texture);
	}
}
