
#include "scorebar.hpp"
#include "gui.hpp"
#include "snake.hpp"
#include <sstream>

	ScoreBar::ScoreBar(const Gui* gui, Snake* p[max_players], int w, int h)
: m_width(w), m_height(h), m_gui(gui), m_players(p)
{
	int pw = m_width / max_players;

	for(int i = 0; i < max_players; ++i)
	{
		m_parts[i] = NULL;
		m_parts[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, pw, m_height, 24, 0, 0, 0, 0);
		genPlayerPart(i);
	}
}

ScoreBar::~ScoreBar()
{
	for(int i = 0; i < max_players; ++i)
	{
		if(m_parts[i] != NULL)
			SDL_FreeSurface(m_parts[i]);
	}
}

void ScoreBar::blitOn(SDL_Surface* dst, SDL_Rect pos)
{
	int pw = m_width / max_players;
	for(int i = 0; i < max_players; ++i)
	{
		if(m_players[i] != NULL)
			genPlayerPart(i);

		if(m_parts[i] == NULL)
			continue;
		SDL_BlitSurface(m_parts[i], NULL, dst, &pos);

		pos.x += pw;
	}
}

void ScoreBar::genPlayerPart(int p)
{
	if(m_parts[p] == NULL)
		return;

	SDL_Color c = m_gui->getPlayerColor(p);
	SDL_FillRect(m_parts[p], NULL, SDL_MapRGB(m_parts[p]->format, c.r, c.g, c.b));
	SDL_Color fg;
	fg.r = fg.g = fg.b = 0;

	if(m_players[p] != NULL)
	{
		std::ostringstream oss;
		oss << m_players[p]->getScore();
		SDL_Surface* txt = TTF_RenderText_Solid(m_gui->getFont(),
				oss.str().c_str(), fg);

		SDL_Rect pos;
		pos.x = m_parts[p]->w / 2 - txt->w / 2;
		pos.y = m_parts[p]->h / 2 - txt->h / 2;
		SDL_BlitSurface(txt, NULL, m_parts[p], &pos);
		SDL_FreeSurface(txt);
	}
	else
	{
		SDL_Surface* txt = TTF_RenderText_Solid(m_gui->getFont(),
				"No player", fg);
		SDL_Rect pos;
		pos.x = m_parts[p]->w / 2 - txt->w / 2;
		pos.y = m_parts[p]->h / 2 - txt->h / 2;
		SDL_BlitSurface(txt, NULL, m_parts[p], &pos);
		SDL_FreeSurface(txt);
	}
}


