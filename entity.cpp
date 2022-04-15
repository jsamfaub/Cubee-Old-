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
	cout<<"rect.x"<<rectangle.x<<endl;
	cout<<"rect.y"<<rectangle.y<<endl;
	cout<<"x:"<<posx<<endl;
	cout<<"y:"<<posy<<endl;
	cout<<"w:"<<width<<endl;
	cout<<"h:"<<height<<endl;
}
void entity::renderEntity(int x, int y) {
	rectangle.x = x;
	rectangle.y = y;
	SDL_RenderCopy(mRenderer, entityTexture, NULL, &rectangle);
}
entity::entity(int w, int h, int x, int y)
{
	rectangle.w =width=w;
	rectangle.h =height=h;
	rectangle.x = posx = x;
	rectangle.y = posy = y;
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
	rectangle.x = posx = x;
}

void entity::sety(int y)
{
	rectangle.y = posy = y;
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

bool entity::near(int playerx,int playery,int distanceParameter){
	if( abs(posx-playerx)<=distanceParameter&&abs(posy-playery)<=distanceParameter) {
		return true;
	}else
		return false;
}
void entity::followPlayer(int x,int y){
	if(posx<x){
		velx=walkingVel;
	}else if(posx>x){
		velx=-walkingVel;
	}else{
		velx=0;
	}
}
void entity::move(int numOfBlocks,rect** blocks){
	posx+=velx;
	//for(int i=0;i<numOfBlocks;i++){
	//	if(collidedOver(blocks[i])){
	//		return;
	//	}
	//}
	//posy+=vely;
	return;
}
bool entity::collidedOver(rect* block){
	if((posy+height)+1==block->gety()){
		if(posx>block->getx()+block->getw()){
			return false;
		}
		if(posx+width<block->getx()){
			return false;
		}
		return true;
	}else{
		return false;
	}
	return false;
}
bool entity::hitBy(entity *e){
	if( (posx+width)<e->getx() ){
		return false;
	}
	if( posx>(e->getx()+e->getw()) ){
		return false;
	}
	if( (posy+height)<e->gety() ){
		return false;
	}
	if( posy>(e->gety()+e->geth()) ){
		return false;
	}
	return true;
}
