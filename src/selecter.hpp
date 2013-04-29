
#ifndef DEF_SELECTER
#define DEF_SELECTER

#include <SDL.h>
#include <vector>
#include "config.hpp"
#include "gui/playersel.hpp"
class Button;
class Gui;
class LocalControler;

class Selecter
{
	public:
		Selecter(Gui* g, SDL_Surface* scr);
		~Selecter();

		bool run();

	private:
		Gui* m_gui;
		SDL_Surface* m_scr;

		Button* m_play;
		Button* m_cancel;
		PlayerSelect* m_sels[max_players];

		std::vector<LocalControler*> m_conts;

		void loadSels();
		void loadConts();
};

#endif

