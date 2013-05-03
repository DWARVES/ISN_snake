
#include <iostream>
#include <SDL.h>
#include "config.hpp"
#include "bonus.hpp"
#include "gui.hpp"
#include "menu.hpp"
#include "music.hpp"
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	// Chargement de la SDL
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		std::cerr << "Erreur au chargement de la SDL : " << SDL_GetError() << std::endl;
		return 1;
	}

	// Ouverture de la fenêtre
	SDL_WM_SetCaption("SankekanS", NULL);
	SDL_Surface* scr = SDL_SetVideoMode(widthMap * sizeTile, heightMap * sizeTile + 25, 24, SDL_DOUBLEBUF);
	if(scr == NULL)
	{
		std::cerr << "Erreur à l'ouverture de la fenêtre : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Chargement des bonus
	fs::path bonPath(rcdir);
	bonPath /= bonus_subdir;
	Bonus::loadAll(bonPath.c_str());

	// Chargement de la Gui
	Gui gui;
	if(gui.getFont() == NULL
			|| gui.background() == NULL)
	{
		std::cerr << "Erreur au chargement de la gui." << std::endl;
		SDL_Quit();
		return 1;
	}

	// Chargement de la musique
	Music mus;
	mus.playMusic();

	// Lancement du jeu
	Menu menu(&gui, scr);
	if(!menu.run())
		std::cout << "Jeu fermé afin sa fin." << std::endl;

	// Libérations diverses
	Bonus::freeAll();
	SDL_Quit();
	return 0;
}

