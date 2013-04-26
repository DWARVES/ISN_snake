
#include "menu.hpp"
#include "button.hpp"
#include "gui.hpp"
#include "server.hpp"
#include <iostream>

	Menu::Menu(Gui* g, SDL_Surface* scr)
: m_gui(g), m_scr(scr),
	m_bret(NULL), m_bplay(NULL), m_bjoin(NULL)
{
	SDL_Rect pos;
	pos.x = m_scr->w / 2;
	pos.y = m_scr->h / 2 - 50;

	m_bplay = new Button("New game", m_gui);
	m_bplay->setPos(pos);

	pos.y += 100;
	m_bjoin = new Button("Join game", m_gui);
	m_bjoin->setPos(pos);

	pos.y += 150;
	m_bret = new Button("Back", m_gui);
	m_bret->setPos(pos);
}

Menu::~Menu()
{
	if(m_bret != NULL)
		delete m_bret;
	if(m_bplay != NULL)
		delete m_bplay;
	if(m_bjoin != NULL)
		delete m_bjoin;
}

bool Menu::run()
{
	bool continuer = true;
	SDL_Event ev;
	SDL_Rect m;

	while(continuer)
	{
		// Affichage
		SDL_FillRect(m_scr, NULL, SDL_MapRGB(m_scr->format, 0, 0, 0));
		SDL_BlitSurface(m_gui->background(), NULL, m_scr, NULL);
		m_bplay->blitOn(m_scr);
		m_bjoin->blitOn(m_scr);
		m_bret->blitOn(m_scr);
		SDL_Flip(m_scr);

		// Évènements
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					return false;
					break;
				case SDL_MOUSEMOTION:
					m.x = ev.motion.x;
					m.y = ev.motion.y;
					m_bplay->mouse(m);
					m_bjoin->mouse(m);
					m_bret->mouse(m);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m.x = ev.button.x;
					m.y = ev.button.y;

					if(m_bplay->clicked(m))
					{
						Server srv(m_gui, m_scr);
						if(!srv.run())
							return false;
					}
					else if(m_bjoin->clicked(m))
					{
						std::cout << "Join game !" << std::endl;
					}
					else if(m_bret->clicked(m))
						continuer = false;
					break;
				default:
					break;
			}
		}
	}

	return true;
}


