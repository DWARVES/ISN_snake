
#include "keyboardcontroler.hpp"
#include "snake.hpp"

int KeyboardControler::m_count = 0;

	KeyboardControler::KeyboardControler()
: Controler(), m_valid(SDLK_UNKNOWN)
{
	++m_count;
}

KeyboardControler::~KeyboardControler()
{
	--m_count;
}

bool KeyboardControler::validEvent(const SDL_Event& ev) const
{
	if(m_count > 1) // Un seul clavier autorisé
		return false;

	if(ev.type != SDL_KEYDOWN)
		return false;

	switch(ev.key.keysym.sym)
	{
		case SDLK_UP:
		case SDLK_DOWN:
		case SDLK_RIGHT:
		case SDLK_LEFT:
			m_valid = ev.key.keysym.sym;
			return true;
		default:
			return false;
	};
}

bool KeyboardControler::stillValid(const SDL_Event& ev) const
{
	if(ev.type != SDL_KEYUP)
		return true;

	if(ev.key.keysym.sym == m_valid)
		return false;
	else
		return true;
}

bool KeyboardControler::processEvent(const SDL_Event& ev)
{
	if(ev.type != SDL_KEYDOWN)
		return false;

	switch(ev.key.keysym.sym)
	{
		case SDLK_UP:
			m_mv = &Snake::moveUp;
			break;
		case SDLK_DOWN:
			m_mv = &Snake::moveDown;
			break;
		case SDLK_RIGHT:
			m_mv = &Snake::moveRight;
			break;
		case SDLK_LEFT:
			m_mv = &Snake::moveLeft;
			break;
		default:
			return false;
	};
	return true;
}

Controler* KeyboardControler::duplicate() const
{
	return new KeyboardControler;
}



