
#ifndef DEF_KEYBOARDCONTROLER
#define DEF_KEYBOARDCONTROLER

#include "localcontroler.hpp"
#include <SDL.h>

class KeyboardControler : public LocalControler
{
	public:
		KeyboardControler();
		virtual ~KeyboardControler();

		// Utilisés lors de la détection des périphériques
		virtual bool validEvent(const SDL_Event& ev) const; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const; // Teste si le nouvel ev n'annule pas le premier

		virtual bool processEvent(const SDL_Event& ev);
		virtual LocalControler* duplicate() const; // Crée un nouveau du même type

	private:
		static int m_count; // Ne peut y avoir qu'un seul clavier
		mutable SDLKey m_valid; // ev de validation
};

#endif

