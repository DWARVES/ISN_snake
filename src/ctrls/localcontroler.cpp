
#include "localcontroler.hpp"
#include "snake.hpp"

LocalControler::LocalControler()
	: Controler(), m_mv(&Snake::moveRight)
{}

LocalControler::~LocalControler()
{}

void LocalControler::move()
{
	(m_snk->*m_mv)();
}


