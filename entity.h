#pragma once
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
	SDL_Rect rect;

	int id;

	void setTexturePath(char*path);
	void renderEntity(int x, int y);
private:
	SDL_Texture *entityTexture = NULL;
	int posx, posy;
	int width, height;
	SDL_Rect collider;
};

