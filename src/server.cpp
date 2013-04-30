
#include "server.hpp"
#include "snake.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "scorebar.hpp"
#include "over.hpp"
#include "keyboardcontroler.hpp"

	Server::Server(Gui* g, SDL_Surface* scr, LocalControler* c[max_players])
: m_g(g), m_scr(scr), m_conts(c),
	m_map(NULL), m_sb(NULL)
{
	m_map = new Map(g);

	SDL_Rect bg;
	bg.x = scr->w / 4 / sizeTile;
	bg.y = scr->h / 4 / sizeTile;
	for(int i = 0; i < max_players; ++i)
	{
		if(m_conts[i] != NULL)
		{
			SDL_Rect pos;
			pos.x = bg.x * (i%2 == 0 ? 1 : 3);
			pos.y = bg.y * (i < 2 ? 1 : 3);

			m_snks[i] = m_conts[i]->getSnake();
			if(m_snks[i] == NULL)
				m_conts[i]->clearSnake();
			m_conts[i]->loadSnake(m_map, pos, i);
			m_snks[i] = m_conts[i]->getSnake();
		}
		else
			m_snks[i] = NULL;
	}

	m_sb = new ScoreBar(g, m_snks, scr->w, 25);
	m_sbp.y = scr->h - 25;
	m_sbp.x = 0;
}

Server::~Server()
{
	delete m_map;
	delete m_sb;
}

bool Server::run()
{
	// Initialisation
	bool continuer = true;
	bool end = false; // Signale arrêt prématuré
	SDL_Event ev;
	Uint32 sltime = SDL_GetTicks(); // Pour le déplacement du serpent
	Uint32 bltime = sltime; // Pour les bonus

	// Mainloop
	while(continuer)
	{
		// Affichage
		SDL_FillRect(m_scr, NULL, SDL_MapRGB(m_scr->format, 0, 0, 0));
		m_map->blitOn(m_scr, NULL);
		m_sb->blitOn(m_scr, m_sbp);
		for(int i = 0; i < max_players; ++i)
		{
			if(m_snks[i] != NULL)
				m_snks[i]->blitOn(m_scr, NULL);
		}
		SDL_Flip(m_scr);

		// Évènements
		while(SDL_PollEvent(&ev))
		{
			bool check = true;
			for(int i = 0; i < max_players; ++i)
			{
				if(m_conts[i] != NULL
						&& m_conts[i]->processEvent(ev))
				{
					check = false;
					continue;
				}
			}
			if(!check)
				continue;

			switch(ev.type)
			{
				case SDL_QUIT:
					return false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_q:
							continuer = false;
							end = true;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}

		// Mise à jour des structures
		// Bonus
		if(SDL_GetTicks() - bltime > 500)
		{
			m_map->addBonus();
			bltime = SDL_GetTicks();
		}
		// Serpents
		if(SDL_GetTicks() - sltime > 100)
		{
			for(int i = 0; i < max_players; ++i)
			{
				if(m_conts[i] == NULL)
					continue;
				m_conts[i]->move();
				if(m_snks[i]->isDead())
					continuer = false;
			}
			sltime = SDL_GetTicks();
		}
	}

	if(!end)
	{
		GameOver ov(m_g, m_scr, m_snks);
		if(!ov.run())
			return false;
	}

	return true;
}


