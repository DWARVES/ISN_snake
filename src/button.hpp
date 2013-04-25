
#ifndef DEF_BUTTON
#define DEF_BUTTON

#include <SDL.h>
class Gui;

class Button
{
	public:
		Button(const char* text, Gui* gui);
		~Button();

		void setPos(SDL_Rect pos);
		void mouse(SDL_Rect m); // Met en valeur sous la souris
		bool clicked(SDL_Rect m) const;

		void blitOn(SDL_Surface* dst) const;

	private:
		SDL_Rect m_pos; // Position du centre
		SDL_Surface* m_txt;
		bool m_hl; // highlight

		SDL_Rect cRect() const;
		bool col(SDL_Rect sc, SDL_Rect p) const;
};

#endif

