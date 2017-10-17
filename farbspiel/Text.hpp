#ifndef TEXT_HPP
#define TEXT_HPP

#include "SDL.h"
#include "SDL_ttf.h"
#include "Framework.hpp"
#include <string>

using namespace std;

class CText
{
public:

	CText();
	CText(const CText &other);
	CText &operator=(const CText &other);
	~CText();
	bool Init( string font,
		   int ptSize,
		   string text,
		   unsigned char r,
		   unsigned char g,
		   unsigned char b,
		   int alpha );
	bool Init(string font, int ptSize, string text, SDL_Color rgb, int alpha);
	void setText(string text);
	void setSize(int ptSize);
	void setPos(int x, int y);
	void setMiddle(int x, int y);
	void setColor( unsigned char r, unsigned char g, unsigned char b, int alpha);
	void setColor(SDL_Color color, int alpha);
	void Render();
	SDL_Rect getRect();

private:
	bool m_bInitialized;

	SDL_Renderer* m_pScreen;
	SDL_Texture* m_pTexture;
	TTF_Font* m_pFont;
	string m_sText;
	string m_sFontName;
	SDL_Color m_Color;
	int m_Alpha;
	int m_ptSize;
	SDL_Rect m_destRect;
};

#endif
