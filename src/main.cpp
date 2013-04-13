
#include <iostream>
#include <SDL.h>
#include "bonus.hpp"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(600, 480, 24, 0);
	Bonus::loadAll("rc/");

	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
	SDL_Rect pos;
	pos.x = pos.y = 0;
	for(size_t i = 0; i < bonus.size(); ++i)
	{
		SDL_Surface* img = bonus[i]->getImg();
		SDL_BlitSurface(img, NULL, ecran, &pos);
		pos.x += img->w;
		if(pos.x >= ecran->w)
		{
			pos.x = 0;
			pos.y += 100;
		}
	}
	SDL_Flip(ecran);

	SDL_Event ev;
	SDL_WaitEvent(&ev);
	while(ev.type != SDL_QUIT)
		SDL_WaitEvent(&ev);

	Bonus::freeAll();
	SDL_Quit();
	return 0;
}

