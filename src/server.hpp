
#ifndef DEF_SERVER
#define DEF_SERVER

#include "config.hpp"
#include <SDL.h>

class Gui;
class Snake;
class Controler;
class Map;
class ScoreBar;

class Server
{
	public:
		Server(Gui* g, SDL_Surface* scr, Controler* c[max_players]);
		~Server();

		bool run(); // Si renvoie false, programme doit s'arrêter

	private:
		Gui* m_g;
		SDL_Surface* m_scr;

		Controler** m_conts;
		Snake* m_snks[max_players];
		int m_alive;
		bool m_onlyone; // Un seul joueur
		Map* m_map;
		ScoreBar* m_sb;
		SDL_Rect m_sbp;
};
#endif
