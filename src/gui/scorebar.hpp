
#ifndef DEF_SCOREBAR
#define DEF_SCOREBAR

#include "config.hpp"
#include <SDL.h>
class Gui;
class Snake;

class ScoreBar
{
	public:
		ScoreBar(const Gui* gui, Snake* p[max_players], int w, int h);
		~ScoreBar();

		void blitOn(SDL_Surface* dst, SDL_Rect pos);

	private:
		const int m_width;
		const int m_height;
		const Gui* m_gui;

		Snake** m_players;
		SDL_Surface* m_parts[max_players];

		void genPlayerPart(int p);
};

#endif

