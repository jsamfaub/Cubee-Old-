#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "init.h"
#include <iostream>
#include "player.h"
#include "LWindow.h"
#include "gdefines.h"
using namespace std;

extern SDL_Renderer* mRenderer;
extern LWindow gWindow[3];
extern int screenwidth;
extern int screenheight;

bool init()
{

	bool success = 1;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "Error1" << endl;
		success = 0;
	}
	else
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			cout << "Error2" << endl;
			success = 0;
		}
		else
		{
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				cout << "Error3" << endl;
				success = 0;
			}
			else
			{
				/*if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
				{
					cout << "texture filtering failed to enable!" << endl;
				}*/

				if (!gWindow[0].init())
				{
					success = false;
				}
			}
		}
	}

	return success;
}

/*
				if (!gWindow.init())
				{
					cout << "Error4" << endl;
					success = 0;
				}
				else
				{
					mRenderer = gWindow.createRenderer();
					if (mRenderer == NULL)
					{
						cout << "Error5" << endl;
						success = 0;
					}
				}*/
