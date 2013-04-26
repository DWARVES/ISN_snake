
#include "entry.hpp"
#include "gui.hpp"
#include <SDL_ttf.h>

Entry::Entry(Gui* g, int w)
	: m_txt(" "), m_gui(g), m_w(w)
{}

bool Entry::processEvent(const SDL_Event& ev)
{
	if(ev.type != SDL_KEYDOWN)
		return false;

	if(m_txt == " ")
		m_txt.clear();

	SDLKey sym = ev.key.keysym.sym;

	// Les lettres de a à z
	if(sym >= SDLK_a
			&& sym <= SDLK_z)
	{
		char c = (char) sym;
		SDLMod mod = SDL_GetModState();
		if(mod & KMOD_RSHIFT
				|| mod & KMOD_LSHIFT
				|| mod & KMOD_CAPS)
		{
				c -= 'a';
				c += 'A';
		}

		m_txt += c;
	}
	// Les nombres
	else if(sym >= SDLK_0
			&& sym <= SDLK_9)
		m_txt += sym - SDLK_0 + '0';
	else if(sym >= SDLK_KP0
			&& sym <= SDLK_KP9)
		m_txt += sym - SDLK_KP0 + '0';
	// Les caractères spéciaux
	else if(sym == SDLK_SPACE)
		m_txt += ' ';
	else if(sym == SDLK_SLASH
			|| sym == SDLK_KP_DIVIDE)
		m_txt += '/';
	else if(sym == SDLK_PERIOD
			|| sym == SDLK_KP_PERIOD)
		m_txt += '.';
	// Les touches d'édition
	else if(sym == SDLK_BACKSPACE
			|| sym == SDLK_DELETE)
	{
		if(m_txt.size() > 0)
			m_txt.resize(m_txt.size() - 1);
		if(m_txt.empty())
			m_txt = ' ';
	}
	else
	{
		if(m_txt.empty())
			m_txt = ' ';
		return false;
	}

	return true;
}

void Entry::blitOn(SDL_Surface* dst, SDL_Rect pos)
{
	SDL_Surface* txt = TTF_RenderText_Solid(m_gui->getFont(),
			m_txt.c_str(), m_gui->fgColor());
	if(txt == NULL)
		return;

	if(m_w == 0)
		pos.w = txt->w + 4;
	else
		pos.w = m_w;
	pos.h = txt->h + 4;

	SDL_Color bgc = m_gui->bgColor();
	SDL_FillRect(dst, &pos, SDL_MapRGB(dst->format, bgc.r, bgc.g, bgc.b));

	pos.x += 2;
	pos.y += 2;

	SDL_Rect part;
	part.x = txt->w - pos.w + 4;
	if(part.x < 0) part.x = 0;
	part.y = 0;
	part.w = pos.w - 4;
	part.h = pos.h - 4;
	SDL_BlitSurface(txt, &part, dst, &pos);
}

