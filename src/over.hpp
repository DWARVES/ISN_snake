
#ifndef DEF_OVER
#define DEF_OVER

#include "config.hpp"
#include <SDL.h>

class Gui;
class Snake;
class Button;

class GameOver
{
	public:
		GameOver(Gui* g, SDL_Surface* s, Snake* snks[max_players]);
		~GameOver();

		bool run();

	private:
		Gui* m_gui;
		SDL_Surface* m_scr;

		SDL_Surface* m_txt; // "Player X won !"
		SDL_Surface* m_scores[max_players]; // "Player X : scr"
		Button* m_back;

		void createScr(Snake* snks[max_players], int idx);
		int computeWin(Snake* snks[max_players]) const;
};

#endif

