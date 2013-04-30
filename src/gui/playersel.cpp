
#include "playersel.hpp"
#include "gui.hpp"
#include "button.hpp"
#include "progress.hpp"
#include "controler.hpp"
#include <sstream>
#include <SDL_ttf.h>

PlayerSelect::PlayerSelect(Gui* g, SDL_Rect r, int id)
	: m_gui(g), m_rect(r), m_id(id), m_cont(NULL), m_busy(false), m_ready(false),
	m_player(NULL), m_mustc(NULL), m_wait(NULL), m_cancel(NULL), m_ltime(0)
{
	std::ostringstream txt;
	txt << "Player " << id;
	m_player = TTF_RenderText_Blended(m_gui->getFont(), txt.str().c_str(), m_gui->getPlayerColor(id));

	m_mustc = TTF_RenderText_Blended(m_gui->getFont(),
			"Press any touch ...",
			m_gui->fgColor());

	m_wait = new Progress(m_gui);
	m_wait->setMax(player_time); // Le nombre millisecondes du chargement
	m_wait->setSize(m_rect.w / 2, 20);

	m_cancel = new Button("Cancel", m_gui);
	SDL_Rect pos;
	pos.x = m_rect.x + m_rect.w / 2;
	pos.y = m_rect.y + m_rect.h / 4 * 3;
	m_cancel->setPos(pos);
}
		
PlayerSelect::~PlayerSelect()
{
	if(m_player != NULL)
		SDL_FreeSurface(m_player);
	if(m_mustc != NULL)
		SDL_FreeSurface(m_mustc);
	if(m_wait != NULL)
		delete m_wait;
	if(m_cancel != NULL)
		delete m_cancel;
	if(m_cont != NULL)
		delete m_cont;
}

void PlayerSelect::blitOn(SDL_Surface* dst)
{
	SDL_Rect pos = m_rect;
	SDL_Color fgc = m_gui->getPlayerColor(m_id);
	SDL_FillRect(dst, &pos, SDL_MapRGB(dst->format, fgc.r, fgc.g, fgc.b));

	pos.x += 10; pos.y += 10;
	pos.w -= 20; pos.h -= 20;
	SDL_FillRect(dst, &pos, SDL_MapRGB(dst->format, 0, 0, 0));

	pos.x = m_rect.x + m_rect.w / 2 - m_player->w / 2;
	pos.y = m_rect.y + m_rect.h / 4 - m_player->h / 2;
	SDL_BlitSurface(m_player, NULL, dst, &pos);

	if(m_ready)
		m_cancel->blitOn(dst);
	else if(m_busy)
	{
		pos.x = m_rect.x + m_rect.w / 4;
		pos.y = m_rect.y + m_rect.h / 4 * 3 - 10;
		m_wait->blitOn(dst, pos);

		Uint32 time = SDL_GetTicks() - m_ltime;
		if(time > player_time)
			m_ready = true;
		else
			m_wait->set(time);
	}
	else
	{
		pos.x = m_rect.x + m_rect.w / 2 - m_mustc->w / 2;
		pos.y = m_rect.y + m_rect.h / 4 * 3 - m_mustc->h / 2;
		SDL_BlitSurface(m_mustc, NULL, dst, &pos);
	}
}

void PlayerSelect::checkEvent(std::vector<Controler*>* ctrls, const SDL_Event& ev)
{
	if(m_ready)
	{
		SDL_Rect pos;
		if(ev.type == SDL_MOUSEBUTTONDOWN)
		{
			pos.x = ev.button.x;
			pos.y = ev.button.y;
			if(m_cancel->clicked(pos))
			{
				delete m_cont;
				m_cont = NULL;
				m_ready = m_busy = false;
				m_wait->set(0);
			}
		}
		else if(ev.type == SDL_MOUSEMOTION)
		{
			pos.x = ev.motion.x;
			pos.y = ev.motion.y;
			m_cancel->mouse(pos);
		}
	}
	else if(m_busy)
	{
		if(!m_cont->stillValid(ev))
		{
			delete m_cont;
			m_cont = NULL;
			m_busy = false;
			m_wait->set(0);
			return;
		}
	}
	else
	{
		for(size_t i = 0; i < ctrls->size(); ++i)
		{
			if(ctrls->at(i)->validEvent(ev))
			{
				m_cont = ctrls->at(i);
				ctrls->at(i) = m_cont->duplicate();
				m_busy = true;
				m_ltime = SDL_GetTicks();
			}
		}
	}
}

bool PlayerSelect::ready() const
{
	return m_ready;
}

bool PlayerSelect::busy() const
{
	return m_busy;
}

Controler* PlayerSelect::getControler() const
{
	return m_cont;
}


