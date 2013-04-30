
#include "controler.hpp"
#include "snake.hpp"

Controler::Controler()
	: m_snk(NULL), m_mv(&Snake::moveRight)
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

void Controler::clearSnake()
{
	if(m_snk != NULL)
		delete m_snk;
	m_snk = NULL;
}

void Controler::move()
{
	(m_snk->*m_mv)();
}


