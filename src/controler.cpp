
#include "controler.hpp"
#include "snake.hpp"
#define NULL 0

Controler::Controler()
	: m_snk(NULL)
{}

Controler::~Controler()
{
	if(m_snk != NULL)
		delete m_snk;
}

void Controler::loadSnake(Map* map, const SDL_Rect& pos, int idx)
{
	m_snk = new Snake(map, pos, idx);
}

Snake* Controler::getSnake() const
{
	return m_snk;
}


