#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "SDL.h"

class CSquare
{
public:
	CSquare();
	void Create(int x, int y, int size, int r, int g, int b, bool rightOne);
	void Draw(SDL_Renderer *renderer);
	void animate(SDL_Renderer *renderer, int opacity);
	void Destroy();
	SDL_Rect getPos() { return m_position; }
	bool Exists() { return m_bExists; }
	bool isCorrect() { return m_bRightOne; }

	SDL_Rect m_position;
	int m_r, m_g, m_b;

private:
	bool m_bExists;


	bool m_bRightOne;
};

#endif
