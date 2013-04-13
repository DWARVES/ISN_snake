
#include <iostream>
#include <SDL.h>
#include "config.hpp"
#include "bonus.hpp"
#include "map.hpp"

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(widthMap * sizeTile, heightMap * sizeTile, 24, SDL_DOUBLEBUF);
	Bonus::loadAll(rcdir);

	srand(time(NULL));
	Map map;

	SDL_Event ev;
	SDL_WaitEvent(&ev);
	bool continuer = true;
	while(continuer)
	{
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		map.blitOn(ecran, NULL);
		SDL_Flip(ecran);

		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
				case SDL_QUIT:
					continuer = false;
					break;
				case SDL_MOUSEBUTTONDOWN: 
					{
						unsigned int x = ev.button.x / sizeTile;
						unsigned int y = ev.button.y / sizeTile;
						if(map.testCase(x, y) == Map::EMPTY)
							std::cout << "Empty case." << std::endl;
						else if(map.testCase(x, y) == Map::BONUS)
						{
							Bonus* bon = map.getBonusAt(x, y);
							std::cout << "Bonus : \n"
								<< "\tPts : " << bon->getPts()
								<< "\n\tLength : " << bon->getLength()
								<< std::endl;
						}
					}
					break;
				case SDL_KEYDOWN:
					switch(ev.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						case SDLK_q:
							continuer = false;
							break;
						case SDLK_SPACE:
							map.addBonus();
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}

	Bonus::freeAll();
	SDL_Quit();
	return 0;
}

