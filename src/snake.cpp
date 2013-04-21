
#include "snake.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <algorithm>
#include <boost/filesystem/path.hpp>

#include "map.hpp"
#include "config.hpp"
#include "bonus.hpp"
#include "utilities.hpp"

namespace fs = boost::filesystem;

	Snake::Snake(Map* map, const SDL_Rect& begin)
: m_map(map), m_toadd(0), m_score(0), m_dead(false),
	m_ltime(SDL_GetTicks()), m_step(0), m_first(NULL), m_last(NULL)
{
	// Création des premières cases
	m_first = new Case;
	m_first->dprev = Case::NONE;
	m_first->prev = NULL;
	m_first->dnext = Case::LEFT;
	m_first->next = new Case;
	m_first->x = begin.x;
	m_first->y = begin.y;

	Case* scd = m_first->next;
	scd->dprev = Case::RIGHT;
	scd->prev = m_first;
	scd->dnext = Case::NONE;
	scd->next = NULL;
	m_last = scd;

	m_map->addWall(m_first->x, m_first->y);
	SDL_Rect plast = begin;
	incrementPos(&plast, -1, 0);
	m_last->x = plast.x;
	m_last->y = plast.y;
	m_map->addWall(plast.x, plast.y);

	// Chargement de l'image des tiles
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			m_tiles[i][j][0] = m_tiles[i][j][1] = NULL;

	fs::path path(rcdir);
	path /= snake_subdir;
	SDL_Surface* tile = IMG_Load(path.string().c_str());
	if(tile == NULL)
		return;

	SDL_Surface* tmp = SDL_DisplayFormatAlpha(tile);
	if(tmp == NULL)
		return;
	SDL_FreeSurface(tile);
	tile = tmp;

	// Conversion en tiles
	for(int x = 0; x < 4; ++x)
	{
		for(int angle = 0; angle < 4; ++angle)
		{
			for(int step = 0; step < 2; ++step)
			{
				SDL_Rect part;
				part.x = x * sizeTile;
				part.y = step * sizeTile;
				part.w = part.h = sizeTile;

				SDL_Surface* tmp = SDL_CreateRGBSurface(tile->flags, sizeTile, sizeTile, tile->format->BitsPerPixel, 0, 0, 0, 0);
				if(tmp == NULL)
					return;

				SDL_BlitSurface(tile, &part, tmp, NULL);
				m_tiles[x][angle][step] = rotTile(tmp, angle);
				SDL_FreeSurface(tmp);
			}
		}
	}
	SDL_FreeSurface(tile);
}

Snake::~Snake()
{
	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 4; ++j)
		{
			if(m_tiles[i][j][0] != NULL)
				SDL_FreeSurface(m_tiles[i][j][0]);
			if(m_tiles[i][j][1] != NULL)
				SDL_FreeSurface(m_tiles[i][j][1]);
		}
	}

	Case* actual = m_first;
	while(actual != NULL)
	{
		m_map->deleteWall(actual->x, actual->y);
		Case* tofree = actual;
		actual = actual->next;
		delete tofree;
	}
}

void Snake::moveUp()
{
	decal();

	m_first->next->dprev = Case::UP;
	m_first->dnext = Case::DOWN;
	m_first->dprev = Case::NONE;
	moveFirst(0, -1);
}

void Snake::moveDown()
{
	decal();

	m_first->next->dprev = Case::DOWN;
	m_first->dnext = Case::UP;
	m_first->dprev = Case::NONE;
	moveFirst(0, 1);
}

void Snake::moveLeft()
{
	decal();

	m_first->next->dprev = Case::LEFT;
	m_first->dnext = Case::RIGHT;
	m_first->dprev = Case::NONE;
	moveFirst(-1, 0);
}

void Snake::moveRight()
{
	decal();

	m_first->next->dprev = Case::RIGHT;
	m_first->dnext = Case::LEFT;
	m_first->dprev = Case::NONE;
	moveFirst(1, 0);
}

bool Snake::dead()
{
	Map::ColType col = m_map->testCase(m_first->x, m_first->y);
	if(col == Map::WALL)
		return true;
	else if(col == Map::BONUS)
	{
		Bonus* bon = m_map->getBonusAt(m_first->x, m_first->y);

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
				Case* tofree = actual;
				actual = actual->prev;

				actual->next = NULL;
				actual->dnext = Case::NONE;
				m_last = actual;

				m_map->deleteWall(tofree->x, tofree->y);
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
	// Animation
	if(SDL_GetTicks() - m_ltime > 500)
	{
		m_step = (m_step - 1) * -1;
		m_ltime = SDL_GetTicks();
	}

	SDL_Rect rpos;
	if(pos == NULL)
		rpos.x = rpos.y = 0;
	else
		rpos = *pos;

	Case* actual = m_first;
	while(actual != NULL)
	{
		int part; int angle;
		getRect(&part, &angle, actual->dprev, actual->dnext);

		SDL_Rect cpos;
		cpos.x = actual->x; cpos.y = actual->y;
		cpos.x *= sizeTile;
		cpos.x += rpos.x;
		cpos.y *= sizeTile;
		cpos.y += rpos.y;

		// La rotation
		SDL_BlitSurface(m_tiles[part][angle][m_step], NULL, dst, &cpos);
		actual = actual->next;
	}
}

void Snake::getRect(int* part, int* angle, Case::Dir prev, Case::Dir next) const
{
	// La colonne et l'angle
	if(next == Case::NONE)
	{
		*part = 3; // la queue, 4eme colonne
		switch(prev)
		{
			case Case::LEFT: *angle = 0; break;
			case Case::RIGHT: *angle = 2; break;
			case Case::UP: *angle = 3; break;
			case Case::DOWN: *angle = 1; break;
			default: break;
		}
	}
	else if(prev == Case::NONE)
	{
		*part = 0; // La tête, 1ere colonne
		switch(next)
		{
			case Case::LEFT: *angle = 0; break;
			case Case::RIGHT: *angle = 2; break;
			case Case::UP: *angle = 3; break;
			case Case::DOWN: *angle = 1; break;
			default: break;
		}
	}
	else if(std::abs(prev - next) == 1)
	{
		*part = 2; // Corp droit, 3eme colonne
		if(prev < Case::NONE)
			*angle = 0;
		else
			*angle = 1;
	}
	else
	{
		*part = 1; // Corp courbe, 2nd colonne
		switch(next)
		{
			case Case::LEFT:
				switch(prev)
				{
					case Case::UP: *angle = 2; break;
					case Case::DOWN: *angle = 3; break;
					default: break;
				}
				break;
			case Case::RIGHT:
				switch(prev)
				{
					case Case::UP: *angle = 1; break;
					case Case::DOWN: *angle = 0; break;
					default: break;
				}
				break;
			case Case::UP:
				switch(prev)
				{
					case Case::LEFT: *angle = 2; break;
					case Case::RIGHT: *angle = 1; break;
					default: break;
				}
				break;
			case Case::DOWN:
				switch(prev)
				{
					case Case::LEFT: *angle = 3; break;
					case Case::RIGHT: *angle = 0; break;
					default: break;
				}
				break;
			default: break;
		}
	}
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

void Snake::decal()
{
	if(m_toadd == 0)
	{
		m_map->deleteWall(m_last->x, m_last->y);

		m_last->dprev = m_last->prev->dprev;
		m_last->x = m_last->prev->x;
		m_last->y = m_last->prev->y;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->dnext = actual->prev->dnext;
			actual->dprev = actual->prev->dprev;
			actual->x = actual->prev->x;
			actual->y = actual->prev->y;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
		ncase->dnext = m_first->dnext;
		ncase->prev = m_first;
		ncase->x = m_first->x;
		ncase->y = m_first->y;

		m_first->next->prev = ncase;
		m_first->next = ncase;
		--m_toadd;
	}
}

void Snake::checkDeath()
{
	if(!dead())
		return;
	m_dead = true;
}

void Snake::moveFirst(signed int x, signed int y)
{
	SDL_Rect pos;
	pos.x = m_first->x; pos.y = m_first->y;
	incrementPos(&pos, x, y);

	m_first->x = pos.x;
	m_first->y = pos.y;
	checkDeath();
	m_map->addWall(m_first->x, m_first->y);
}

bool Snake::isDead() const
{
	return m_dead;
}


