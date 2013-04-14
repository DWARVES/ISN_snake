
#include "snake.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include "map.hpp"
#include "config.hpp"
#include "bonus.hpp"

namespace fs = boost::filesystem;

	Snake::Snake(Map* map, const SDL_Rect& begin)
: m_map(map), m_x(begin.x), m_y(begin.y), m_toadd(0), m_score(0),
	m_tile(NULL), m_ltime(SDL_GetTicks()), m_step(true), m_first(NULL), m_last(NULL)
{
	// Création des premières cases
	m_first = new Case;
	m_first->dprev = Case::NONE;
	m_first->prev = NULL;
	m_first->dnext = Case::LEFT;
	m_first->next = new Case;

	Case* scd = m_first->next;
	scd->dprev = Case::RIGHT;
	scd->prev = m_first;
	scd->dnext = Case::NONE;
	scd->next = NULL;
	m_last = scd;

	// Chargement de m_tile
	fs::path path(rcdir);
	path /= snake_subdir;
	m_tile = IMG_Load(path.string().c_str());
	if(m_tile == NULL)
		return;

	SDL_Surface* tmp = SDL_DisplayFormat(m_tile);
	if(tmp == NULL)
		return;
	SDL_FreeSurface(m_tile);
	m_tile = tmp;
}

Snake::~Snake()
{
	if(m_tile != NULL)
		delete m_tile;

	Case* actual = m_first;
	while(actual != NULL)
	{
		Case* tofree = actual;
		actual = actual->next;
		delete tofree;
	}
}

void Snake::moveUp()
{
	if(m_toadd == 0)
	{
		m_last->dprev = m_last->prev->dprev;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->dnext = actual->prev->dnext;
			actual->dprev = actual->prev->dprev;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
		ncase->dnext = m_first->dnext;
		ncase->prev = m_first;
		m_first->next = ncase;
	}

	m_first->next->dprev = Case::UP;
	m_first->dnext = Case::DOWN;
	m_first->dprev = Case::NONE;

	SDL_Rect pos;
	pos.x = m_x; pos.y = m_y;
	incrementPos(&pos, 0, -1);
	m_x = pos.x; m_y = pos.y;
}

void Snake::moveDown()
{
	if(m_toadd == 0)
	{
		m_last->dprev = m_last->prev->dprev;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->dnext = actual->prev->dnext;
			actual->dprev = actual->prev->dprev;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
		ncase->dnext = m_first->dnext;
		ncase->prev = m_first;
		m_first->next = ncase;
	}

	m_first->next->dprev = Case::DOWN;
	m_first->dnext = Case::UP;
	m_first->dprev = Case::NONE;

	SDL_Rect pos;
	pos.x = m_x; pos.y = m_y;
	incrementPos(&pos, 0, 1);
	m_x = pos.x; m_y = pos.y;
}

void Snake::moveLeft()
{
	if(m_toadd == 0)
	{
		m_last->dprev = m_last->prev->dprev;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->dnext = actual->prev->dnext;
			actual->dprev = actual->prev->dprev;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
		ncase->dnext = m_first->dnext;
		ncase->prev = m_first;
		m_first->next = ncase;
	}

	m_first->next->dprev = Case::LEFT;
	m_first->dnext = Case::RIGHT;
	m_first->dprev = Case::NONE;

	SDL_Rect pos;
	pos.x = m_x; pos.y = m_y;
	incrementPos(&pos, -1, 0);
	m_x = pos.x; m_y = pos.y;
}

void Snake::moveRight()
{
	if(m_toadd == 0)
	{
		m_last->dprev = m_last->prev->dprev;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->dnext = actual->prev->dnext;
			actual->dprev = actual->prev->dprev;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
		ncase->dnext = m_first->dnext;
		ncase->prev = m_first;
		m_first->next = ncase;
	}

	m_first->next->dprev = Case::RIGHT;
	m_first->dnext = Case::LEFT;
	m_first->dprev = Case::NONE;

	SDL_Rect pos;
	pos.x = m_x; pos.y = m_y;
	incrementPos(&pos, 1, 0);
	m_x = pos.x; m_y = pos.y;
}

bool Snake::dead()
{
	Map::ColType col = m_map->testCase(m_x, m_y);
	if(col == Map::WALL)
		return true;
	else if(col == Map::BONUS)
	{
		Bonus* bon = m_map->getBonusAt(m_x, m_y);

		int scr = bon->getPts();
		if(scr < 0 
				&& (unsigned int)-scr > m_score)
			m_score = 0;
		else
			m_score += scr;

		int length = bon->getLength();
		if(length < 0) // On supprime les cases
		{
			length *= -1;
			Case* actual = m_last;
			for(int i = 0; i < length
					&& actual != NULL
					&& actual->prev != m_first; // Minimum deux cases
					++i)
			{
				actual->prev->next = NULL;
				Case* tofree = actual;
				actual = actual->prev;
				delete tofree;
			}
		}
		else
			m_toadd += bon->getLength();
	}

	return false;
}

void Snake::blitOn(SDL_Surface* dst, SDL_Rect* pos) const
{
	if(m_tile == NULL)
		return;

	SDL_Rect rpos;
	if(pos == NULL)
		rpos.x = rpos.y = 0;
	else
		rpos = *pos;

	SDL_Rect dec;
	dec.x = dec.y = 0;
	Case* actual = m_first;
	while(actual != NULL)
	{
		SDL_Rect part; int angle;
		getRect(&part, &angle, actual->dprev, actual->dnext);

		SDL_Rect cpos;
		cpos.x = m_x; cpos.y = m_y;
		incrementPos(&cpos, dec.x, dec.y);
		cpos.x *= sizeTile;
		cpos.x += rpos.x;
		cpos.y *= sizeTile;
		cpos.y += rpos.y;

		// La rotation
		SDL_Surface* spart = SDL_CreateRGBSurface(0, sizeTile, sizeTile, 24, 0, 0, 0, 0);
		if(spart != NULL)
		{
			SDL_BlitSurface(m_tile, &part, spart, NULL);
			SDL_Surface* toblit = rotozoomSurface(spart, angle, 1, 0);

			if(toblit != NULL)
			{
				SDL_BlitSurface(toblit, NULL, dst, &cpos);
				SDL_FreeSurface(spart);
				SDL_FreeSurface(toblit);
			}
		}

		switch(actual->dnext)
		{
			case Case::UP: --dec.y; break;
			case Case::DOWN: ++dec.y; break;
			case Case::LEFT: --dec.x; break;
			case Case::RIGHT: ++dec.x; break;
			default: break;
		}
		actual = actual->next;
	}
}

void Snake::getRect(SDL_Rect* dst, int* angle, Case::Dir prev, Case::Dir next) const
{
	// La ligne utilisée
	if(m_step)
		dst->y = 0;
	else
		dst->y = sizeTile;

	// La colonne et l'angle
	if(next == Case::NONE)
	{
		dst->x = sizeTile * 3; // la queue, 4eme colonne
		switch(prev)
		{
			case Case::LEFT: *angle = 0; break;
			case Case::RIGHT: *angle = 180; break;
			case Case::UP: *angle = 90; break;
			case Case::DOWN: *angle = 270; break;
			default: break;
		}
	}
	else if(prev == Case::NONE)
	{
		dst->x = 0; // La tête, 1ere colonne
		switch(next)
		{
			case Case::LEFT: *angle = 0; break;
			case Case::RIGHT: *angle = 180; break;
			case Case::UP: *angle = 90; break;
			case Case::DOWN: *angle = 270; break;
			default: break;
		}
	}
	else if(std::abs(prev - next) < 2)
	{
		dst->x = sizeTile; // Corp droit, 2nd colonne
		if(prev < Case::NONE)
			*angle = 0;
		else
			*angle = 90;
	}
	else
	{
		dst->x = sizeTile * 2; // Corp courbe, 3eme colonne
	}

	// Largeur et hauteur
	dst->w = dst->h = sizeTile;
}

void Snake::incrementPos(SDL_Rect* dst, signed int x, signed int y) const
{
	dst->x += x;
	if(dst->x < 0)
		dst->x = widthMap + dst->x;
	else if(dst->x >= widthMap)
		dst->x -= widthMap;

	dst->y += y;
	if(dst->y < 0)
		dst->y = heightMap + dst->y;
	else if(dst->y >= heightMap)
		dst->y -= heightMap;
}

unsigned int Snake::getScore() const
{
	return m_score;
}
