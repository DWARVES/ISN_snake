
#ifndef DEF_PROGRESS
#define DEF_PROGRESS

class Gui;
#include <SDL.h>

class Progress
{
	public:
		Progress(Gui* g);
		Progress(Gui* g, int m, int w, int h);

		void setMax(int m);
		void setSize(int w, int h);

		void set(int v);
		void blitOn(SDL_Surface* dst, SDL_Rect pos);

	private:
		Gui* m_gui;
		int m_max;
		int m_act;
		int m_w;
		int m_h;
};

#endif

