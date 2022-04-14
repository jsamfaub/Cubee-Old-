#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "entity.h"
#include <iostream>
using namespace std;
extern SDL_Renderer* mRenderer;
extern int gTileSize;
void entity::setTexturePath(char*path) {
	SDL_DestroyTexture(entityTexture);
	SDL_Surface* surface = NULL;
	surface = IMG_Load(path);
	entityTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
	if(entityTexture == NULL)
		printf("Error in loading entity");
	else{
		printf("No error in loading entity\n");
		std::cout<<path<<std::endl;
	}
	SDL_FreeSurface(surface);
}
void entity::printInfo(){
	cout<<"rect.x"<<rect.x<<endl;
	cout<<"rect.y"<<rect.y<<endl;
	cout<<"x:"<<posx<<endl;
	cout<<"y:"<<posy<<endl;
	cout<<"w:"<<width<<endl;
	cout<<"h:"<<height<<endl;
	cout<<"rectxywh:"<<rect.x<<rect.y<<rect.w<<rect.w<<endl;
}
void entity::renderEntity(int x, int y) {
	rect.x = x;
	rect.y = y;
	SDL_RenderCopy(mRenderer, entityTexture, NULL, &rect);
}
entity::entity(int w, int h, int x, int y)
{
	rect.w =width=w;
	rect.h =height=h;
	rect.x = posx = x;
	rect.y = posy = y;
}

void entity::setw(int w)
{
	width = w;
}

void entity::seth(int h)
{
	height = h;
}

void entity::setx(int x)
{
	rect.x = posx = x;
}

void entity::sety(int y)
{
	rect.y = posy = y;
}

int entity::getw()
{
	return width;
}

int entity::geth()
{
	return height;
}

int entity::getx()
{
	return posx;
}

int entity::gety()
{
	return posy;
}

SDL_Rect& entity::getCollider()
{
	collider.x = posx;
	collider.y = posy;
	collider.w = width;
	collider.h = height;

	return collider;
}
