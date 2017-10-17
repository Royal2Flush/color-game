#include "Framework.hpp"
#include "Game.hpp"

int main(int argc, char *argv[])
{
	int xres = 500;
	int yres = 500;

	if (!g_pFramework->Init(xres, yres, 16, false))
		return (0);

	CGame Game;
	Game.Init (xres, yres);
	Game.Run (); //runs till window closed or "esc" pressed
	Game.Quit ();

	//end framework
	g_pFramework->Quit();
	g_pFramework->Del();

	return (0);
}
