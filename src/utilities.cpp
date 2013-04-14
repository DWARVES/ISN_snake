
#include "utilities.hpp"
#include "config.hpp"

typedef SDL_Rect (*Transform)(SDL_Rect); 

SDL_Rect transform90(SDL_Rect pos)
{
	SDL_Rect ret;
	ret.x = sizeTile - pos.y;
	ret.y = ret.x;
	return ret;
}

SDL_Rect transform180(SDL_Rect pos)
{
	SDL_Rect ret;
	ret.x = sizeTile - pos.y;
	ret.y = sizeTile - pos.x;
	return ret;
}

SDL_Rect transform270(SDL_Rect pos)
{
	SDL_Rect ret;
	ret.x = pos.y;
	ret.y = sizeTile - pos.x;
	return ret;
}


SDL_Surface* rotTile(SDL_Surface* tile, int ang)
{
	SDL_Surface* rot = SDL_CreateRGBSurface(0, tile->w, tile->h, 24, 0, 0, 0, 0);
	if(rot == NULL)
		return NULL;

	Transform tr;
	switch(ang)
	{
		case 1: tr = transform90; break;
		case 2: tr = transform180; break;
		case 3: tr = transform270; break;
		default:
			SDL_BlitSurface(tile, NULL, rot, NULL);
			return rot;
	}

	SDL_Rect pos;
	for(pos.x = 0; pos.x < tile->w; ++pos.x)
	{
		for(pos.y = 0; pos.y < tile->h; ++pos.y)
		{
			Uint32 col = getPixel(tile, pos.x, pos.y);
			SDL_Rect npos = tr(pos);
			putPixel(rot, npos.x, npos.y, col);
		}
	}

	return rot;
}

Uint32 getPixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void putPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

