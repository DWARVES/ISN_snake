
#include "button.hpp"
#include "gui.hpp"
#include <SDL_ttf.h>

Button::Button(const char* text, Gui* gui)
	: m_txt(NULL), m_hl(false)
{
	m_pos.x = m_pos.y = 0;

	SDL_Color fg, bg;
	fg.r = fg.b = bg.r = bg.g = bg.b = 0;
	fg.g = 255;
	m_txt = TTF_RenderText_Shaded(gui->getFont(), text, fg, bg);
}

Button::~Button()
{
	if(m_txt != NULL)
		SDL_FreeSurface(m_txt);
}

void Button::setPos(SDL_Rect pos)
{
	m_pos = pos;
}

void Button::mouse(SDL_Rect m)
{
	m_hl = clicked(m);
}

bool Button::clicked(SDL_Rect m) const
{
	return col(cRect(), m);
}

void Button::blitOn(SDL_Surface* dst) const
{
	if(m_txt == NULL)
		return;

	SDL_Rect cr = cRect();
	SDL_Rect pos = cr;

	Uint32 fg = SDL_MapRGB(dst->format, 0, 255, 0);
	Uint32 bg = SDL_MapRGB(dst->format, 0, 0, 0);
	SDL_FillRect(dst, &cr, bg);
	if(m_hl)
	{
		pos.h = 5;
		SDL_FillRect(dst, &pos, fg);
		pos.y += cr.h - 5;
		SDL_FillRect(dst, &pos, fg);
		pos.y = cr.y; pos.w = 5; pos.h = cr.h;
		SDL_FillRect(dst, &pos, fg);
		pos.x += cr.w - 5;
		SDL_FillRect(dst, &pos, fg);
	}

	pos.x = cr.w / 2 - m_txt->w / 2 + cr.x;
	pos.y = cr.h / 2 - m_txt->h / 2 + cr.y;
	SDL_BlitSurface(m_txt, NULL, dst, &pos);
}

SDL_Rect Button::cRect() const
{
	SDL_Rect r;
	r.x = m_pos.x - (m_txt->w+15) / 2;
	r.y = m_pos.y - (m_txt->h+15) / 2;
	r.w = m_txt->w + 15;
	r.h = m_txt->h + 15;

	return r;
}

bool Button::col(SDL_Rect sc, SDL_Rect p) const
{
	if(p.x < sc.x
			|| p.y < sc.y
			|| p.x > sc.x + sc.w
			|| p.y > sc.y + sc.h)
		return false;
	else
		return true;
}

