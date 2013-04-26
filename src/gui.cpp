
#include "gui.hpp"
#include <boost/filesystem/path.hpp>
#include <SDL_image.h>

namespace fs = boost::filesystem;

Gui::Gui()
	: m_font(NULL), m_bg(NULL)
{
	// Les couleurs
	for(int i = 0; i < max_players; ++i)
		m_colors[i].r = m_colors[i].g = m_colors[i].b = 0;
	m_colors[0].r = 255; // Joueur 1 rouge
	m_colors[1].g = 255; // Joueur 2 vert
	m_colors[2].b = 255; // Joueur 3 bleu
	m_colors[3].r = m_colors[3].g = 255; // Joueur 4 jaune

	// TTF
	if(TTF_Init() < 0)
		return;

	// Font
	fs::path p(rcdir);
	p /= gui_subdir;
	p /= "font.ttf";

	m_font = TTF_OpenFont(p.string().c_str(), 20);

	// Chargement du fond d'écran
	fs::path path(rcdir);
	path /= bg_path;
	m_bg = IMG_Load(path.string().c_str());
}

Gui::~Gui()
{
	if(m_font != NULL)
		TTF_CloseFont(m_font);
	TTF_Quit();

	if(m_bg != NULL)
		SDL_FreeSurface(m_bg);
}

TTF_Font* Gui::getFont() const
{
	return m_font;
}

SDL_Color Gui::getPlayerColor(int p) const
{
	if(p < max_players)
		return m_colors[p];
	else
	{
		SDL_Color c;
		c.r = c.g = c.b = 0;
		return c;
	}
}

SDL_Color Gui::fgColor() const
{
	SDL_Color c;
	c.r = c.g = c.b = 100;
	c.g = 255;
	return c;
}

SDL_Color Gui::bgColor() const
{
	SDL_Color c;
	c.r = c.g = c.b = 0;
	return c;
}

SDL_Surface* Gui::background() const
{
	return m_bg;
}



