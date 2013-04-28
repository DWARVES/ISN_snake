
#ifndef DEF_LOCALCONTROLER
#define DEF_LOCALCONTROLER

#include "controler.hpp"
union SDL_Event;

class LocalControler : public Controler // Controler pour les périphériques physiques : clavier, joystick ...
{
	public:
		LocalControler();
		virtual ~LocalControler();

		// Utilisés lors de la détection des périphériques
		virtual bool validEvent(const SDL_Event& ev) const = 0; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const = 0; // Teste si le nouvel ev n'annule pas le premier

		virtual bool processEvent(const SDL_Event& ev) = 0;
		virtual void move();

	protected:
		typedef void (Snake::*Move)();
		Move m_mv;
};

#endif

