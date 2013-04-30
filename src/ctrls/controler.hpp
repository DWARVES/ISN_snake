
#ifndef DEF_CONTROLER
#define DEF_CONTROLER

#include <SDL.h>
class Snake;
class Map;
struct SDL_Rect;

class Controler
{
	public:
		Controler();
		virtual ~Controler();

		virtual void loadSnake(Map* map, const SDL_Rect& pos, int idx);
		virtual void clearSnake();
		Snake* getSnake() const;

		virtual bool validEvent(const SDL_Event& ev) const = 0; // Teste si l'évènement peut être utilisé
		virtual bool stillValid(const SDL_Event& ev) const = 0; // Teste si le nouvel ev n'annule pas le premier
		virtual Controler* duplicate() const = 0; // Crée un nouveau du même type

		virtual bool processEvent(const SDL_Event& ev) = 0;
		virtual void move(); // Déplace le serpent

	protected:
		Snake* m_snk;

		typedef void (Snake::*Move)();
		Move m_mv;
};

#endif

