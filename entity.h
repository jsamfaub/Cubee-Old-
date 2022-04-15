#pragma once
#include "rect.h"
class entity
{
public:
	entity(int,int,int x=100,int y=100);

	void setw(int w);
	void seth(int h);
	void setx(int x);
	void sety(int y);
	int getw();
	int geth();
	int getx();
	int gety();

	SDL_Rect& getCollider();
	SDL_Rect rectangle;

	int id;
	bool near(int playerx,int playery,int distanceParameter);

	void setTexturePath(char*path);
	void renderEntity(int x, int y);
	void printInfo();
	void followPlayer(int x,int y);
	void move(int numOfBlocks, rect** blocks);
	bool hitBy(entity *arm);
	bool collidedOver(rect *block);
private:
	SDL_Texture *entityTexture = NULL;
	int posx, posy;
	int velx, vely=10;
	int width, height;
	SDL_Rect collider;
	int walkingVel=2;
	int fallingVel=10;
};

