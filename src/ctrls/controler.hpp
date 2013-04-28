
#ifndef DEF_CONTROLER
#define DEF_CONTROLER

class Snake;
class Map;
struct SDL_Rect;

class Controler
{
	public:
		Controler();
		virtual ~Controler();

		virtual void loadSnake(Map* map, const SDL_Rect& pos, int idx);
		Snake* getSnake() const;

		virtual void move() = 0; // Déplace le serpent

	protected:
		Snake* m_snk;
};

#endif

