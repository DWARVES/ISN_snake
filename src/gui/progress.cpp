
#include "progress.hpp"
#include "gui.hpp"

Progress::Progress(Gui* g)
	: m_gui(g), m_max(0), m_act(0), m_w(0), m_h(0)
{}

Progress::Progress(Gui* g, int m, int w, int h)
	: Progress(g)
{
	setMax(m);
	setSize(w, h);
}

void Progress::setMax(int m)
{
	m_max = m;
}

void Progress::setSize(int w, int h)
{
	m_w = w;
	m_h = h;
}

void Progress::set(int v)
{
	if(v > m_max)
		m_act = m_max;
	else if(v < 0)
		m_act = 0;
	else
		m_act = v;
}

void Progress::blitOn(SDL_Surface* dst, SDL_Rect pos)
{
	SDL_Color fgc = m_gui->fgColor();
	Uint32 fg = SDL_MapRGB(dst->format, fgc.r, fgc.g, fgc.b);
	SDL_Color bgc = m_gui->bgColor();
	Uint32 bg = SDL_MapRGB(dst->format, bgc.r, bgc.g, bgc.b);

	pos.w = m_w;
	pos.h = m_h;
	SDL_FillRect(dst, &pos, bg);

	pos.x += 1; pos.y += 1;
	pos.w = m_act * m_w / m_max;
	if(pos.w >= 2) pos.w -= 2;
	pos.h -= 2;
	SDL_FillRect(dst, &pos, fg);
}


