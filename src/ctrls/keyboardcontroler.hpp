
#ifndef DEF_KEYBOARDCONTROLER
#define DEF_KEYBOARDCONTROLER

#define NB_KEYS_CONFIGS 3

#include "controler.hpp"
#include <SDL.h>

class KeyboardControler : public Controler
{
	public:
		KeyboardControler();
		virtual ~KeyboardControler();

		// Utilisés lors de la détection des périphériques
		virtual bool validEvent(const SDL_Event& ev) const; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const; // Teste si le nouvel ev n'annule pas le premier

		virtual bool processEvent(const SDL_Event& ev);
		virtual Controler* duplicate() const; // Crée un nouveau du même type

	private:
        mutable int m_keysUsed;
		mutable SDLKey m_valid; // ev de validation

        enum Dirs {
            Left,
            Right,
            Up,
            Down,
            Last
        };
        static SDLKey m_keys[NB_KEYS_CONFIGS][(unsigned int)Last];
        static bool m_used[NB_KEYS_CONFIGS];
        static bool m_init;

        int confFromKey(SDLKey key) const;
        static void init();
};

#endif

