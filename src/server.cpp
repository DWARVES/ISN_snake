
#include "server.hpp"
#include "snake.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "scorebar.hpp"

	Server::Server(Gui* g, SDL_Surface* scr)
: m_g(g), m_scr(scr),
	m_map(NULL), m_sb(NULL)
{
	m_map = new Map(g);
	SDL_Rect bg;
	for(int i = 0; i < max_players; ++i)
	{
		bg.x = widthMap / 4 * (i%2 == 0 ? 1 : 3);
		bg.y = heightMap / 4 * (i < 2 ? 1 : 3);
		m_snks[i] = new Snake(m_map, bg);
	}

	m_sb = new ScoreBar(g, m_snks, scr->w, 25);
	m_sbp.y = scr->h - 25;
	m_sbp.x = 0;
}

Server::~Server()
{
	for(int i = 0; i < max_players; ++i)
	{
		if(m_snks[i] != NULL)
			delete m_snks[i];
	}
	delete m_map;
	delete m_sb;
}

typedef void (Snake::*Move)();
bool Server::run()
{
	// Initialisation
	bool continuer = true;
	bool end = false; // Signale arrêt prématuré
	SDL_Event ev;
	Uint32 sltime = SDL_GetTicks(); // Pour le déplacement du serpent
	Uint32 bltime = sltime; // Pour les bonus
	Move mv(&Snake::moveRight);

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
			switch(ev.type)
			{
				case SDL_QUIT:
					return false;
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_UP:
							mv = &Snake::moveUp;
							break;
						case SDLK_DOWN:
							mv = &Snake::moveDown;
							break;
						case SDLK_LEFT:
							mv = &Snake::moveLeft;
							break;
						case SDLK_RIGHT:
							mv = &Snake::moveRight;
							break;
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
			(m_snks[0]->*mv)();
			if(m_snks[0]->isDead())
				continuer = false;
			sltime = SDL_GetTicks();
		}
	}

	if(!end) {} // TODO game over

	return true;
}

