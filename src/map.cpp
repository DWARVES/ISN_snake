
#include "map.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <boost/filesystem/path.hpp>
#include "bonus.hpp"

namespace fs = boost::filesystem;

Map::Map()
	: m_bg(NULL)
{
	// Initialisation de la map
	for(unsigned int x = 0; x < widthMap; ++x)
	{
		for(unsigned int y = 0; y < heightMap; ++y)
			m_map[x][y].col = EMPTY;
	}

	// Chargement du fond d'écran
	fs::path path(rcdir);
	path /= bg_path;
	m_bg = IMG_Load(path.string().c_str());
}

Map::~Map()
{
	if(m_bg != NULL)
		SDL_FreeSurface(m_bg);
}

Map::ColType Map::testCase(unsigned int x, unsigned int y) const
{
	return m_map[x][y].col;
}

Bonus* Map::getBonusAt(unsigned int x, unsigned int y)
{
	return bonus[ m_map[x][y].bon->idx ];
}

void Map::addBonus()
{
	// Position du bonus
	unsigned int x = rand() % widthMap;
	unsigned int y = rand() % heightMap;
	while(testCase(x,y) != EMPTY)
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
			i = bonus.size();
		}
	}
}

void Map::addWall(unsigned int x, unsigned int y)
{
	m_map[x][y].col = WALL;
}

void Map::deleteWall(unsigned int x, unsigned int y)
{
	m_map[x][y].col = EMPTY;
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
	for(bon_it it = m_bonus.begin(); it != m_bonus.end(); ++it)
	{
		// Mise à jour des valeurs
		if(timeSpent > it->timeleft)
			deleteBonus(it);
		else
		{
			it->timeleft -= timeSpent;
			// Affichage
			rpos.x = it->x * sizeTile;
			rpos.y = it->y * sizeTile;
			SDL_BlitSurface(bonus[it->idx]->getImg(), NULL, dst, &rpos);
		}
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
	m_bonus.push_back(ibon);
}


