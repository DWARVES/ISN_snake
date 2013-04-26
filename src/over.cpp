
#include "over.hpp"
#include "gui.hpp"
#include "snake.hpp"
#include "button.hpp"
#include <sstream>
#include <SDL_ttf.h>

GameOver::GameOver(Gui* g, SDL_Surface* s, Snake* snks[max_players])
	: m_gui(g), m_scr(s), m_txt(NULL), m_back(NULL)
{
	int win = computeWin(snks);
	for(int i = 0; i < max_players; ++i)
		createScr(snks, i);

	std::ostringstream oss;
	oss << "Player " << win + 1 << " won !";
	m_txt = TTF_RenderText_Blended(m_gui->getFont(),
			oss.str().c_str(),
			m_gui->fgColor());

	m_back = new Button("End", m_gui);
	SDL_Rect pos;
	pos.x = m_scr->w / 8 * 7;
	pos.y = m_scr->h / 8 * 7;
	m_back->setPos(pos);
}

GameOver::~GameOver()
{
	for(int i = 0; i < max_players; ++i)
	{
		if(m_scores[i] != NULL)
			SDL_FreeSurface(m_scores[i]);
	}

	if(m_txt != NULL)
		SDL_FreeSurface(m_txt);

	if(m_back != NULL)
		delete m_back;
}

bool GameOver::run()
{
	bool continuer = true;
	SDL_Rect pos;
	SDL_Event ev;

	while(continuer)
	{
		// Affichage
		SDL_FillRect(m_scr, NULL, SDL_MapRGB(m_scr->format, 0, 0, 0));
		SDL_BlitSurface(m_gui->background(), NULL, m_scr, NULL);
		pos.x = m_scr->w / 2 - m_txt->w / 2;
		pos.y = m_scr->h / 4 - m_txt->h / 2;
		SDL_BlitSurface(m_txt, NULL, m_scr, &pos);

		for(int i = 0; i < max_players; ++i)
		{
			pos.x = m_scr->w / 3 * (i%2 + 1) - m_scores[i]->w / 2;
			pos.y = m_scr->h / 6 * (i<2 ? 4 : 5) - m_scores[i]->h / 2;
			SDL_BlitSurface(m_scores[i], NULL, m_scr, &pos);
		}
		m_back->blitOn(m_scr);
		SDL_Flip(m_scr);

		// Évènements
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					return false;
				case SDL_KEYDOWN:
					if(ev.key.keysym.sym == SDLK_q
							|| ev.key.keysym.sym == SDLK_BACKSPACE
							|| ev.key.keysym.sym == SDLK_ESCAPE)
						continuer = false;
					break;
				case SDL_MOUSEMOTION:
					pos.x = ev.motion.x;
					pos.y = ev.motion.y;
					m_back->mouse(pos);
					break;
				case SDL_MOUSEBUTTONDOWN:
					pos.x = ev.button.x;
					pos.y = ev.button.y;
					if(m_back->clicked(pos))
						continuer = false;
					break;
				default:
					break;
			}
		}
	}

	return true;
}

void GameOver::createScr(Snake* snks[max_players], int idx)
{
	std::ostringstream oss;
	if(snks[idx] != NULL)
		oss << "Player " << idx + 1 << " : " << snks[idx]->getScore();
	else
		oss << "No player";

	m_scores[idx] = TTF_RenderText_Blended(m_gui->getFont(),
			oss.str().c_str(),
			m_gui->getPlayerColor(idx));
}

int GameOver::computeWin(Snake* snks[max_players]) const
{
	int w = 0;
	unsigned int max = snks[0]->getScore();

	for(int i = 1; i < max_players && snks[i] != NULL; ++i)
	{
		if(snks[i]->getScore() > max)
		{
			max = snks[i]->getScore();
			w = i;
		}
	}

	return w;
}

