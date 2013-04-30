
#ifndef DEF_JOYSTICKCONTROLER
#define DEF_JOYSTICKCONTROLER

#include "controler.hpp"
#include <vector>
#include <SDL.h>

class JoystickControler : public Controler
{
	public:
		JoystickControler();
		~JoystickControler();

		// Utilisés lors de la détection des périphériques
		virtual bool validEvent(const SDL_Event& ev) const; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const; // Teste si le nouvel ev n'annule pas le premier

		virtual bool processEvent(const SDL_Event& ev);
		virtual Controler* duplicate() const; // Crée un nouveau du même type

	private:
		static std::vector<int> m_used; // Les joysticks utilisés
		static size_t m_count;
		mutable int m_valid;
		mutable int m_joy;

		bool isUsed(int j) const;
		void erase(int j) const;
};

#endif

