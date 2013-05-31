
#ifndef DEF_MENU
#define DEF_MENU

#include <SDL.h>
class Gui;
class Button;

class Menu
{
	public:
		Menu(Gui* g, SDL_Surface* scr);
		~Menu();

		bool run();

	private:
		Gui* m_gui;
		SDL_Surface* m_scr;

		Button* m_bret;
		Button* m_bplay;
		SDL_Surface* m_title;
};

#endif

