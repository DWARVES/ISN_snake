
#include "snake.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <algorithm>
#include <sstream>

#include "map.hpp"
#include "config.hpp"
#include "bonus.hpp"
#include "utilities.hpp"

	Snake::Snake(Map* map, const SDL_Rect& begin, int id)
: m_map(map), m_toadd(0), m_score(0), m_dead(false), m_loaded(true),
	m_ltime(SDL_GetTicks()), m_step(0), m_first(NULL), m_last(NULL)
{
	// Création des premières cases
	m_first = new Case;
	m_first->prev = NULL;
	m_first->next = new Case;
	m_first->x = begin.x;
	m_first->y = begin.y;

	Case* scd = m_first->next;
	scd->prev = m_first;
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

	std::ostringstream path;
	path << rcdir << snake_subdir << id << ".png";
	SDL_Surface* tile = IMG_Load(path.str().c_str());
	if(tile == NULL)
	{
		m_loaded = false;
		return;
	}

	SDL_Surface* tmp = SDL_DisplayFormatAlpha(tile);
	SDL_FreeSurface(tile);
	if(tmp == NULL)
	{
		m_loaded = false;
		return;
	}
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
				{
					m_loaded = false;
					return;
				}

				SDL_BlitSurface(tile, &part, tmp, NULL);
				m_tiles[x][angle][step] = rotTile(tmp, angle);
				SDL_FreeSurface(tmp);
			}
		}
	}
	SDL_FreeSurface(tile);
}

bool Snake::isLoaded() const
{
	return m_loaded;
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
	moveFirst(0, -1);
}

void Snake::moveDown()
{
	decal();
	moveFirst(0, 1);
}

void Snake::moveLeft()
{
	decal();
	moveFirst(-1, 0);
}

void Snake::moveRight()
{
	decal();
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
		getRect(&part, &angle,
				getDir(actual, actual->prev),
				getDir(actual, actual->next));

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

void Snake::getRect(int* part, int* angle, Dir prev, Dir next) const
{
	// La colonne et l'angle
	if(next == NONE)
	{
		*part = 3; // la queue, 4eme colonne
		switch(prev)
		{
			case LEFT: *angle = 0; break;
			case RIGHT: *angle = 2; break;
			case UP: *angle = 3; break;
			case DOWN: *angle = 1; break;
			default: break;
		}
	}
	else if(prev == NONE)
	{
		*part = 0; // La tête, 1ere colonne
		switch(next)
		{
			case LEFT: *angle = 0; break;
			case RIGHT: *angle = 2; break;
			case UP: *angle = 3; break;
			case DOWN: *angle = 1; break;
			default: break;
		}
	}
	else if(std::abs(prev - next) == 1)
	{
		*part = 2; // Corp droit, 3eme colonne
		if(prev < NONE)
			*angle = 0;
		else
			*angle = 1;
	}
	else
	{
		*part = 1; // Corp courbe, 2nd colonne
		switch(next)
		{
			case LEFT:
				switch(prev)
				{
					case UP: *angle = 2; break;
					case DOWN: *angle = 3; break;
					default: break;
				}
				break;
			case RIGHT:
				switch(prev)
				{
					case UP: *angle = 1; break;
					case DOWN: *angle = 0; break;
					default: break;
				}
				break;
			case UP:
				switch(prev)
				{
					case LEFT: *angle = 2; break;
					case RIGHT: *angle = 1; break;
					default: break;
				}
				break;
			case DOWN:
				switch(prev)
				{
					case LEFT: *angle = 3; break;
					case RIGHT: *angle = 0; break;
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

		m_last->x = m_last->prev->x;
		m_last->y = m_last->prev->y;
		Case* actual = m_last->prev;
		while(actual != m_first)
		{
			actual->x = actual->prev->x;
			actual->y = actual->prev->y;
			actual = actual->prev;
		}
	}
	else
	{
		Case* ncase = new Case;
		ncase->next = m_first->next;
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

Snake::Dir Snake::getDir(const Case* c1, const Case* c2) const
{
	if(c1 == NULL || c2 == NULL)
		return NONE;

	if(c1->x > c2->x)
	{
		if(c2->x == 0 && c1->x != 1)
			return RIGHT;
		else
			return LEFT;
	}
	else if(c1->x < c2->x)
	{
		if(c1->x == 0 && c2->x != 1)
			return LEFT;
		else
			return RIGHT;
	}
	else if(c1->y > c2->y)
	{
		if(c2->y == 0 && c1->y != 1)
			return DOWN;
		else
			return UP;
	}
	else
	{
		if(c1->y == 0 && c2->y != 1)
			return UP;
		else
			return DOWN;
	}
}

