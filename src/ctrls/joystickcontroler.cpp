
#include "joystickcontroler.hpp"
#include "snake.hpp"

#define SENSIBILITY 26000

size_t JoystickControler::m_count = 0;
std::vector<int> JoystickControler::m_used;
std::vector<SDL_Joystick*> m_allJoys; // Doivent être ouverts pour être utilisés

JoystickControler::JoystickControler()
	: Controler(), m_valid(-1), m_joy(-1)
{
	++m_count;

	SDL_JoystickEventState(SDL_ENABLE);
	if(m_allJoys.size() == 0)
	{
		m_allJoys.resize(SDL_NumJoysticks());
		for(size_t i = 0; i < m_allJoys.size(); ++i)
			m_allJoys[i] = SDL_JoystickOpen(i);
	}
}

JoystickControler::~JoystickControler()
{
	erase(m_joy);

	--m_count;
	if(m_count == 0)
	{
		for(size_t i = 0; i < m_allJoys.size(); ++i)
			SDL_JoystickClose(m_allJoys[i]);
		m_allJoys.clear();
	}
}

bool JoystickControler::validEvent(const SDL_Event& ev) const
{
	if((long)m_used.size() >= SDL_NumJoysticks()) // Pas plus de controlers que de joysticks
		return false;

	if(ev.type != SDL_JOYBUTTONDOWN)
		return false;

	if(isUsed(ev.jbutton.which))
		return false;

	m_valid = ev.jbutton.button;
	m_joy = ev.jbutton.which;
	m_used.push_back(m_joy);
	return true;
}

bool JoystickControler::stillValid(const SDL_Event& ev) const
{
	if(ev.type != SDL_JOYBUTTONUP)
		return true;

	if(ev.jbutton.which == m_joy
			&& ev.jbutton.button == m_valid)
	{
		m_valid = -1;
		erase(m_joy);
		m_joy = -1;
		return false;
	}

	return true;

}

bool JoystickControler::processEvent(const SDL_Event& ev)
{
	if(m_joy < 0)
		return false;

	switch(ev.type)
	{
		case SDL_JOYAXISMOTION:
			if(ev.jaxis.which != m_joy)
				return false;

			if(ev.jaxis.axis % 2 == 1) // Impair = vertical
			{
				if(ev.jaxis.value > SENSIBILITY)
					m_mv = &Snake::moveDown;
				else if(ev.jaxis.value < -SENSIBILITY)
					m_mv = &Snake::moveUp;
			}
			else
			{
				if(ev.jaxis.value > SENSIBILITY)
					m_mv = &Snake::moveRight;
				else if(ev.jaxis.value < -SENSIBILITY)
					m_mv = &Snake::moveLeft;
			}
			break;
		case SDL_JOYHATMOTION:
			if(ev.jhat.which != m_joy)
				return false;

			switch(ev.jhat.value)
			{
				case SDL_HAT_UP:
					m_mv = &Snake::moveUp;
					break;
				case SDL_HAT_DOWN:
					m_mv = &Snake::moveDown;
					break;
				case SDL_HAT_LEFT:
					m_mv = &Snake::moveLeft;
					break;
				case SDL_HAT_RIGHT:
					m_mv = &Snake::moveRight;
					break;
				default:
					return false;
			};
			break;
		default:
			return false;
	};

	return true;
}

bool JoystickControler::isUsed(int j) const
{
	for(size_t i = 0; i < m_used.size(); ++i)
	{
		if(m_used[i] == j)
			return true;
	}
	return false;
}

void JoystickControler::erase(int j) const
{
	for(std::vector<int>::iterator it = m_used.begin(); it != m_used.end(); ++it)
	{
		if(*it == j)
		{
			m_used.erase(it);
			return;
		}
	}
}

Controler* JoystickControler::duplicate() const
{
	return new JoystickControler;
}


