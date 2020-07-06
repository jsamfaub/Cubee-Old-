#include <SDL2/SDL.h>
#include "checkCollision.h"

bool checkCollision(SDL_Rect &a, SDL_Rect &b)
{
	int leftA = a.x;
	int leftB = b.x;
	int rightA = a.x + a.w;
	int rightB = b.x + b.w;
	int topA = a.y;
	int topB = b.y;
	int bottomA = a.y + a.h;
	int bottomB = b.y + b.h;

	if (topA >= bottomB)
	{
		return false;
	}
	if (topB >= bottomA)
	{
		return false;
	}
	if(leftA >= rightB)
	{
		return false;
	}
	if (leftB >= rightA)
	{
		return false;
	}

	return true;
}
