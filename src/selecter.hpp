
#ifndef DEF_SELECTER
#define DEF_SELECTER

#include <SDL.h>
#include <vector>
#include "config.hpp"
#include "gui/playersel.hpp"
class Button;
class Gui;
class Controler;
class Mode;

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

		std::vector<Controler*> m_conts;

		void loadSels();
		void loadConts();
        Mode* selMode();
};

#endif

