
#ifndef DEF_JOYSTICKCONTROLER
#define DEF_JOYSTICKCONTROLER

#include "localcontroler.hpp"
#include <vector>
#include <SDL.h>

class JoystickControler : public LocalControler
{
	public:
		JoystickControler();
		~JoystickControler();

		// Utilisés lors de la détection des périphériques
		virtual bool validEvent(const SDL_Event& ev) const; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const; // Teste si le nouvel ev n'annule pas le premier

		virtual bool processEvent(const SDL_Event& ev);

	private:
		static std::vector<int> m_used; // Les joysticks utilisés
		mutable int m_valid;
		mutable int m_joy;

		bool isUsed(int j) const;
		void erase(int j) const;
};

#endif

