
#ifndef DEF_UTILITIES
#define DEF_UTILITIES

#include <SDL/SDL.h>

SDL_Surface* rotTile(SDL_Surface* tile, int ang); // Ang est entre 0 et 3 : il est ensuite multiplié par 90
Uint32 getPixel(SDL_Surface* surface, int x, int y);
void putPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

#endif

