#include "Square.hpp"

CSquare::CSquare()
{
	m_bExists = false;
}

void CSquare::Create(int x, int y, int size, int r, int g, int b, bool rightOne)
{
	m_bExists = true;
	m_position.x = x;
	m_position.y = y;
	m_position.w = size;
	m_position.h = size;
	m_r = r;
	m_g = g;
	m_b = b;
	m_bRightOne = rightOne;
}

void CSquare::Destroy()
{
	m_bExists = false;
}

void CSquare::Draw(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, m_r, m_g, m_b, 255);
	SDL_RenderFillRect(renderer, &m_position);
}

void CSquare::animate(SDL_Renderer *renderer, int opacity)
{
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, m_r, m_g, m_b, opacity);
        SDL_RenderFillRect(renderer, &m_position);
}
