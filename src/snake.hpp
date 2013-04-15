
#ifndef DEF_SNAKE
#define DEF_SNAKE

class Map;
struct SDL_Surface;
struct SDL_Rect;

class Snake
{
	public:
		Snake(Map* map, const SDL_Rect& begin);
		~Snake();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		void blitOn(SDL_Surface* dst, SDL_Rect* pos) const;
		unsigned int getScore() const;

		struct Death { // Exception lancée pour signaler la mort du serpent
			unsigned int score;
		};

	private:
		Map* m_map;
		unsigned int m_toadd;
		unsigned int m_score;
		SDL_Surface* m_tile;

		mutable unsigned int m_ltime;
		mutable bool m_step;

		struct Case {
			enum Dir{LEFT, RIGHT,
				NONE, // Utilisé pour les calculs
				UP, DOWN};
			Dir dnext;
			Case* next;
			Dir dprev;
			Case* prev;
			unsigned int x, y;
		};
		Case* m_first;
		Case* m_last;

		void getRect(SDL_Rect* dst, int* angle, Case::Dir prev, Case::Dir next) const; // Donne la partie de m_tile à blitter
		void incrementPos(SDL_Rect* dst, signed int x, signed int y) const; // Gère le retour d'un côté quand on sort de l'autre
		void decal();
		void checkDeath();
		bool dead(); // Indique si le serpent est mort et utilise les bonus
};

#endif

