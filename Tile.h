#pragma once
#include "gdefines.h"
class Tile
{
public:
	Tile(int x, int y, int tileType);

	void render(SDL_Rect& camera);

	int getType();

	SDL_Rect getBox();
private:
	SDL_Rect mBox;

	int mType;
};

