#pragma once
#include <string>

class ui
{
public:
	ui(std::string foreGroundPath, std::string scoreDisplayPath);
	~ui();

	void close();

	void render(int score);

	int points();

	void scoreup();
	void inputScore(int num);
private:
	char *paths[10] = { "data/graphics/numbers/zero.png","data/graphics/numbers/one.png","data/graphics/numbers/two.png","data/graphics/numbers/three.png","data/graphics/numbers/four.png","data/graphics/numbers/five.png","data/graphics/numbers/six.png","data/graphics/numbers/seven.png","data/graphics/numbers/eight.png","data/graphics/numbers/nine.png" };

	int score = 0;
	SDL_Texture *foreGroundTexture;
	SDL_Texture *scoreDisplayTextureLeft = NULL;
	SDL_Texture *scoreDisplayTextureRight = NULL;
};
