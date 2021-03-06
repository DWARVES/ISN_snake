
#ifndef DEF_SNAKE
#define DEF_SNAKE

class Map;
struct SDL_Surface;
struct SDL_Rect;

class Snake
{
	public:
		Snake(Map* map, const SDL_Rect& begin, int id);
		bool isLoaded() const;
		~Snake();

		void moveUp();
		void moveDown();
		void moveLeft();
		void moveRight();

		void blitOn(SDL_Surface* dst, SDL_Rect* pos) const;
		bool died(int still); // still = nombre de snakes encore vivants
        bool alive();
		void applyDeath();
		unsigned int getScore(bool life = false) const;

	private:
		int m_id;
		Map* m_map;
		unsigned int m_toadd;
		unsigned int m_score;
        unsigned int m_deadScore;
		bool m_dead;
		mutable bool m_lastd; // m_dead avant l'appel à died
		SDL_Surface* m_tiles[4][4][2][2]; // [partie][rotation][anim][dead]
		bool m_loaded;
        unsigned int m_createdTime;

		mutable unsigned int m_ltime;
		mutable int m_step;

		struct Case {
			Case* next;
			Case* prev;
			unsigned int x, y;
		};
		Case* m_first;
		Case* m_last;

		enum Dir{LEFT, RIGHT,
			NONE, // Utilisé pour les calculs
			UP, DOWN};
		void getRect(int* part, int* angle, Dir prev, Dir next) const; // Donne la partie de m_tile à blitter
		Dir getDir(const Case* c1, const Case* c2) const;

		void incrementPos(SDL_Rect* dst, signed int x, signed int y) const; // Gère le retour d'un côté quand on sort de l'autre
		void decal();
		void moveFirst(signed int x, signed int y);
		void checkDeath();
		bool dead(); // Indique si le serpent est mort et utilise les bonus
        void free(); // Free everything
        void resizeToOne(const SDL_Rect& begin);
        void revive();
};

#endif

