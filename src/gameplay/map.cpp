
#include "map.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <boost/filesystem/path.hpp>
#include "bonus.hpp"
#include "gui.hpp"

namespace fs = boost::filesystem;

Map::Map(Gui* g)
	: m_bg(NULL)
{
	// Initialisation de la map
	for(unsigned int x = 0; x < widthMap; ++x)
	{
		for(unsigned int y = 0; y < heightMap; ++y)
		{
			m_map[x][y].col = EMPTY;
			m_map[x][y].state = NOTHING;
		}
	}

	m_bg = g->background();
}

Map::ColType Map::testCase(unsigned int x, unsigned int y, int id) const
{
	if(m_map[x][y].col == WALL && m_map[x][y].state != NOTHING)
	{
		Status s;
		switch(id)
		{
			case 0: s = SNK0; break;
			case 1: s = SNK1; break;
			case 2: s = SNK2; break;
			case 3: s = SNK3; break;
			default: s = NOTHING; break;
		}
		if(m_map[x][y].state == s)
			return EMPTY;
		else
			return WALL;
	}
	else
		return m_map[x][y].col;
}

Bonus* Map::getBonusAt(unsigned int x, unsigned int y)
{
	Bonus* ret =  bonus[ m_map[x][y].bon->idx ];
	deleteBonus(m_map[x][y].bon);
	return ret;
}

void Map::addBonus()
{
	// Position du bonus
	unsigned int x = rand() % widthMap;
	unsigned int y = rand() % heightMap;
	while(m_map[x][y].col != EMPTY)
	{
		x = rand() % widthMap;
		y = rand() % heightMap;
	}

	// Type de bonus
	int prob = rand() % 1000;
	int state = 0;
	for(size_t i = 0; i < bonus.size(); ++i)
	{
		state += bonus[i]->getFact();
		if(prob <= state)
		{
			addBonus(x, y, i);
			return;
		}
	}
}

void Map::addWall(unsigned int x, unsigned int y)
{
	m_map[x][y].col = WALL;
	deleteStatus(x, y);
}

void Map::deleteWall(unsigned int x, unsigned int y)
{
	m_map[x][y].col = EMPTY;
	deleteStatus(x, y);
}

void Map::blitOn(SDL_Surface* dst, SDL_Rect* pos)
{
	SDL_Rect rpos;
	if(pos == NULL)
		rpos.x = rpos.y = 0;
	else
		rpos = *pos;

	if(m_bg != NULL)
		SDL_BlitSurface(m_bg, NULL, dst, &rpos);

	Uint32 timeSpent = SDL_GetTicks() - m_ltime;
	for(bon_it it = m_bonus.begin(); it != m_bonus.end();)
	{
		// Mise à jour des valeurs
		if(timeSpent > it->timeleft)
		{
			bon_it toErase = it;
			++it;
			deleteBonus(toErase);
		}
		else
		{
			it->timeleft -= timeSpent;
			// Affichage
			rpos.x = it->x * sizeTile;
			rpos.y = it->y * sizeTile;
			SDL_BlitSurface(bonus[it->idx]->getImg(), NULL, dst, &rpos);
			++it;
		}
	}

	for(auto it = m_status.begin(); it != m_status.end();)
	{
		auto toErase = it;
		++it;
		addWall(toErase->first, toErase->second);
	}

	m_ltime = SDL_GetTicks();
}

void Map::deleteBonus(bon_it it)
{
	m_map[it->x][it->y].col = EMPTY;
	m_bonus.erase(it);
}

void Map::addBonus(unsigned int x, unsigned int y, unsigned int bon)
{
	InterBonus ibon;
	ibon.x = x; ibon.y = y;
	ibon.idx = bon;
	ibon.timeleft = bonus[bon]->getTime();

	m_map[x][y].bon = m_bonus.insert(m_bonus.end(), ibon);
	m_map[x][y].col = BONUS;
}

void Map::setStatus(unsigned int x, unsigned int y, int id)
{
	if(m_map[x][y].col == WALL)
		return;
	else if(m_map[x][y].state != NOTHING)
	{
		addWall(x, y);
		return;
	}

	Status s;
	switch(id)
	{
		case 0: s = SNK0; break;
		case 1: s = SNK1; break;
		case 2: s = SNK2; break;
		case 3: s = SNK3; break;
		default: s = NOTHING; break;
	}
	m_map[x][y].state = s;
	m_status.insert(m_status.end(), std::pair<unsigned int,unsigned int>(x,y));
}

void Map::deleteStatus(unsigned int x, unsigned int y)
{
	if(m_status.begin() == m_status.end()
			|| m_map[x][y].state == NOTHING)
		return;

	m_map[x][y].state = NOTHING;

	auto it = std::find(m_status.begin(), m_status.end(), std::pair<unsigned int, unsigned int>(x,y));
	if(it != m_status.end())
		m_status.erase(it);
}

