#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "close.h"
#include "LWindow.h"
#include "gdefines.h"

extern SDL_Renderer* mRenderer;
extern LWindow gWindow[TOTAL_WINDOWS];

void close()
{
	if (mRenderer != NULL)
	{
		SDL_DestroyRenderer(mRenderer);
	}

	for (int i = 0; i < TOTAL_WINDOWS; i++)
	{
		gWindow[i].free();
	}

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
