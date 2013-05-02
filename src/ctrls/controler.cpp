
#include "controler.hpp"
#include "snake.hpp"
#include "config.hpp"
#include <sstream>
#include <SDL_image.h>

Controler::Controler()
	: m_snk(NULL), m_img(NULL), m_mv(&Snake::moveRight)
{}

Controler::~Controler()
{
	if(m_snk != NULL)
		delete m_snk;
	if(m_img != NULL)
		SDL_FreeSurface(m_img);
}

void Controler::loadSnake(Map* map, const SDL_Rect& pos, int idx)
{
	clearSnake();
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

void Controler::clearMove()
{
	m_mv = &Snake::moveRight;
}

void Controler::move()
{
	(m_snk->*m_mv)();
}

SDL_Surface* Controler::picture() const
{
	return m_img;
}

void Controler::loadPict(std::string name)
{
	std::ostringstream path;
	path << rcdir << ctrl_subdir << name;
	m_img = IMG_Load(path.str().c_str());
}

