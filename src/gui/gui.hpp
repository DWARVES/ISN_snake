
#ifndef DEF_GUI
#define DEF_GUI

#include <SDL_ttf.h>
#include "config.hpp"

class Gui
{
	public:
		Gui();
		~Gui();

		TTF_Font* getFont() const; // Si NULL, erreur d'initialisation
		SDL_Color getPlayerColor(int p) const; // La couleur du joueur
		SDL_Color fgColor() const;
		SDL_Color bgColor() const;
		SDL_Surface* background() const;

	private:
		TTF_Font* m_font;
		SDL_Color m_colors[max_players];
		SDL_Surface* m_bg;
};

#endif

