#include "Framework.hpp"

// Init
//
// Aufgabe: Framework initialisieren
//
bool CFramework::Init(int ScreenWidth, int ScreenHeight,
	int ColorDepth, bool bFullscreen)
{
	// Alle Systeme der SDL initialisieren
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO) == -1)
	{
		cout << "SDL could not be initialized. Errormessage:\n" << SDL_GetError() << endl;

		Quit();

		return (false);
	}

	// Vollbild oder Fenstermodus einstellen
	if (bFullscreen == true)
	{
		m_pWindow = SDL_CreateWindow("Farbspiel",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			0, 0,
			SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("Farbspiel",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			ScreenWidth,
			ScreenHeight,
			0);
	}

	if (m_pWindow == nullptr)
	{
		cout << "Window could not be created. Errormessage:\n" << SDL_GetError() << endl;
		Quit();
		return(false);
	}
	else
	{
		m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	}

	if (m_pRenderer == nullptr)
	{
		cout << "Renderer could not be created. Errormessage:\n" << SDL_GetError() << endl;
		Quit();
		return(false);
	}
	else
	{
		SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
		SDL_RenderSetLogicalSize(m_pRenderer, ScreenWidth, ScreenHeight);
	}

	if (TTF_Init() != 0)
	{
		cout << "Error initializing TTF" << endl;
		return false;
	}

/*	if (Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT , 2, 512))
	{
		cout << "Unable to open audio!\n";
		Quit();
		return(false);
	}*/


	// Zeiger auf internes Array für Tastaturstatus ermitteln
	m_pKeystate = SDL_GetKeyboardState(NULL);

	// Alles ging glatt, also true zurückliefern
	return (true);

} // Init


// Quit
//
// Aufgabe: Framework (SDL) beenden
//
void CFramework::Quit()
{
	// SDL beenden
	//Mix_CloseAudio();
	//Mix_Quit();
	TTF_Quit();
	SDL_Quit();

} // Quit


// Update
//
// Aufgabe: Timer und Keyboardstatus updaten
//
void CFramework::Update()
{
	// Timer updaten
	g_pTimer->Update();

	// Tastaturstatus ermitteln
	SDL_PumpEvents();

} // Update
// Keydown
//
// Aufgabe: Tastendruck abfragen
//
bool CFramework::KeyDown(int Key_ID)
{
	// Prüfen, ob Taste gedrückt ist
	return m_pKeystate[Key_ID];

} // KeyDown


// Clear
//
// Aufgabe: Buffer	löschen
//
void CFramework::Clear()
{
	SDL_RenderClear(m_pRenderer);
	/*// Buffer (Surface) mit Hintergrundfarbe füllen
	SDL_FillRect(m_pScreen, NULL,
		SDL_MapRGB(m_pScreen->format, 255, 255, 194));*/

} // Clear


// Flip
//
// Aufgabe: Surface umschalten (flippen)
//
void CFramework::Render()
{
	// Surface umschalten
	SDL_RenderPresent(m_pRenderer);
} // Flip
