
#ifndef DEF_SERVER
#define DEF_SERVER

#include "config.hpp"
#include <SDL.h>

class Gui;
class Snake;
class Map;
class ScoreBar;

class Server
{
	public:
		Server(Gui* g, SDL_Surface* scr);
		~Server();

		bool run(); // Si renvoie false, programme doit s'arrêter

	private:
		Gui* m_g;
		SDL_Surface* m_scr;

		Snake* m_snks[max_players];
		Map* m_map;
		ScoreBar* m_sb;
		SDL_Rect m_sbp;
};
#endif
