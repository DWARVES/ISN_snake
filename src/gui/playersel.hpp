
#ifndef DEF_PLAYERSEL
#define DEF_PLAYERSEL

#include <SDL.h>
#include <vector>
class Gui;
class Button;
class Progress;
class Controler;

class PlayerSelect
{
	public:
		PlayerSelect(Gui* g, SDL_Rect r, int id);
		~PlayerSelect();

		void blitOn(SDL_Surface* dst);
		void checkEvent(std::vector<Controler*>* ctrls, const SDL_Event& ev);

		bool ready() const;
		bool busy() const;
		Controler* getControler() const;

	private:
		Gui* m_gui;
		SDL_Rect m_rect;
		int m_id;

		Controler* m_cont;
		bool m_busy; // en attente
		bool m_ready; // chargé

		SDL_Surface* m_player; // Player X
		SDL_Surface* m_mustc; // Press any touch
		Progress* m_wait;
		Button* m_cancel;
		Uint32 m_ltime;
};

#endif

