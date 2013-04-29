
#include "selecter.hpp"
#include "server.hpp"
#include "gui/button.hpp"
#include "gui/gui.hpp"
#include "misc/music.hpp"

#include "ctrls/localcontroler.hpp"
#include "ctrls/joystickcontroler.hpp"
#include "ctrls/keyboardcontroler.hpp"

#include <iostream>

Selecter::Selecter(Gui* g, SDL_Surface* scr)
	: m_gui(g), m_scr(scr), m_play(NULL), m_cancel(NULL)
{
	SDL_Rect pos;
	m_play = new Button("Play", m_gui);
	pos.x = m_scr->w - 50;
	pos.y = m_scr->h - 20;
	m_play->setPos(pos);

	m_cancel = new Button("Cancel", m_gui);
	pos.x = 50;
	pos.y = m_scr->h - 20;
	m_cancel->setPos(pos);

	loadSels();
	loadConts();
}

Selecter::~Selecter()
{
	if(m_play != NULL)
		delete m_play;
	if(m_cancel != NULL)
		delete m_cancel;

	for(int i = 0; i < max_players; ++i)
	{
		if(m_sels[i] != NULL)
			delete m_sels[i];
	}

	for(size_t i = 0; i < m_conts.size(); ++i)
	{
		if(m_conts[i] != NULL)
			delete m_conts[i];
	}
}

bool Selecter::run()
{
	bool continuer = true;
	SDL_Event ev;

	while(continuer)
	{
		// Affichage
		SDL_FillRect(m_scr, NULL, SDL_MapRGB(m_scr->format, 0, 0, 0));
		SDL_BlitSurface(m_gui->background(), NULL, m_scr, NULL);
		for(int i = 0; i < max_players; ++i)
			m_sels[i]->blitOn(m_scr);
		m_play->blitOn(m_scr);
		m_cancel->blitOn(m_scr);
		SDL_Flip(m_scr);

		// Évènements
		while(SDL_PollEvent(&ev))
		{
			// Selecters
			bool first = true;
			for(int i = 0; i < max_players; ++i)
			{
				if(m_sels[i]->ready()
						|| m_sels[i]->busy())
					m_sels[i]->checkEvent(&m_conts, ev);
				else if(first)
				{
					m_sels[i]->checkEvent(&m_conts, ev);
					first = false;
				}
			}

			// Menu
			SDL_Rect m;
			switch(ev.type)
			{
				case SDL_QUIT:
					return false;
				case SDL_MOUSEMOTION:
					m.x = ev.motion.x;
					m.y = ev.motion.y;
					m_play->mouse(m);
					m_cancel->mouse(m);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m.x = ev.button.x;
					m.y = ev.button.y;

					if(m_cancel->clicked(m))
					{
						music->playSound(Music::CANCEL);
						continuer = false;
					}
					else if(m_play->clicked(m))
					{
						music->playSound(Music::CANCEL);

						LocalControler* conts[max_players];
						for(int i = 0; i < max_players; ++i)
							conts[i] = m_sels[i]->getControler();

						Server srv(m_gui, m_scr, conts);
						if(!srv.run())
							return false;
						break;
					}
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_q:
						case SDLK_ESCAPE:
							continuer = false;
							break;
						default:
							break;
					}
				default:
					break;
			}
		}
	}

	return true;
}

void Selecter::loadSels()
{
	SDL_Rect pos;
	for(int i = 0; i < max_players; ++i)
	{
		pos.x = 20 + (m_scr->w / 2 - 10) * (i % 2);
		pos.y = 20 + (m_scr->h / 2 - 20) * (i<2 ? 0 : 1);
		pos.w = m_scr->w / 2 - 30;
		pos.h = m_scr->h / 2 - 40;

		m_sels[i] = NULL;
		m_sels[i] = new PlayerSelect(m_gui, pos, i);
	}
}

void Selecter::loadConts()
{
	m_conts.resize(2);
	m_conts[0] = new KeyboardControler;
	m_conts[1] = new JoystickControler;
}


