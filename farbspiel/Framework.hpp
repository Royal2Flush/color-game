#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <iostream>
#include <string>
#include "Singleton.hpp"
#include "Timer.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
//#include "SDL_mixer.h"

using namespace std;

#define g_pFramework CFramework::Get()

class CFramework : public TSingleton<CFramework>
{
public:
	bool  Init(int ScreenWidth, int ScreenHeight,
		int ColorDepth, bool bFullscreen);
	void  Quit();
	void  Update();
	void  Clear();
	void  Render();
	bool  KeyDown(int Key_ID);
	SDL_Renderer *GetScreen() { return m_pRenderer; }

private:
	SDL_Window *m_pWindow;
	SDL_Renderer *m_pRenderer;

	//SDL_Surface *m_pScreen;   // Surface für den Screen
	const Uint8       *m_pKeystate; // Array für aktuellen Tastaturstatus
	string m_sTexturePack;
};

#endif
