#pragma once
class rect
{
public:
	rect();

	void render(int x, int y);

	int getx();
	int gety();
	int getw();
	int geth();
	void setx(int x);
	void sety(int y);
	void setw(int width);
	void seth(int height);

	bool isFilled();
	void fill();
	void empty();

	void setCol(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);

	SDL_Rect& getCollider();

	void texture();
private:
	int posx, posy, w, h;
	SDL_Rect dest;
	bool filled;
	SDL_Color col;
};

