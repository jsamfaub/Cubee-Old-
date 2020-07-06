#pragma once
#include <string>
class background
{
public:
	background(int sw, int sh);
	~background();

	bool loadTexture(std::string path);
	void render(SDL_Rect camera);
	void close();



private:
	SDL_Texture* texture;
	int w, h, posx, posy;
	SDL_Rect dest;
};