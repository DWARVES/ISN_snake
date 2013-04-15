
#include <iostream>
#include <SDL.h>
#include "config.hpp"
#include "bonus.hpp"
#include "map.hpp"
#include "snake.hpp"
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* ecran = SDL_SetVideoMode(widthMap * sizeTile, heightMap * sizeTile, 24, SDL_DOUBLEBUF);
	fs::path bonPath(rcdir);
	bonPath /= bonus_subdir;
	Bonus::loadAll(bonPath.c_str());

	srand(time(NULL));
	Map map;

	SDL_Rect pos;
	pos.x = 20;
	pos.y = 10;
	Snake snk(&map, pos);

	SDL_Event ev;
	SDL_WaitEvent(&ev);
	bool continuer = true;
	while(continuer)
	{
		SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
		map.blitOn(ecran, NULL);
		snk.blitOn(ecran, NULL);
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
						case SDLK_UP:
							snk.moveUp();
							break;
						case SDLK_DOWN:
							snk.moveDown();
							break;
						case SDLK_RIGHT:
							snk.moveRight();
							break;
						case SDLK_LEFT:
							snk.moveLeft();
							break;
						default:
							break;
					}
					if(snk.dead())
						continuer = false;
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

