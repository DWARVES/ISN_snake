
#ifndef DEF_ENTRY
#define DEF_ENTRY

#include <string>
#include <SDL.h>
class Gui;

class Entry
{
	public:
		Entry(Gui* g, int w = 0);

		bool processEvent(const SDL_Event& ev);
		void blitOn(SDL_Surface* dst, SDL_Rect pos);
		std::string getText() const;

	private:
		std::string m_txt;
		Gui* m_gui;
		int m_w;
};

#endif

