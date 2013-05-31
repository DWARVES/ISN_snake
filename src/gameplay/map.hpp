
#ifndef DEF_MAP
#define DEF_MAP

#include <list>
#include "config.hpp"

class Bonus;
struct SDL_Surface;
struct SDL_Rect;
class Gui;

class Map
{
	public:
		Map(Gui* g);

		enum ColType{BONUS, // Bonus
			WALL, // Corp d'un serpent
			EMPTY};

		ColType testCase(unsigned int x, unsigned int y, int id) const;
		Bonus* getBonusAt(unsigned int x, unsigned int y); // Supprime le bonus, le retour ne doit pas être libéré
		void addBonus(); // Ajoute aléatoirement un bonus

		void setStatus(unsigned int x, unsigned int y, int id);
		void addWall(unsigned int x, unsigned int y);
		void deleteWall(unsigned int x, unsigned int y);
		
		void blitOn(SDL_Surface* dst, SDL_Rect* pos); // Blitte le bg et les images des bonus, update aussi le temps des bonus -> supprime ceux dont le temps est écoulé
		
	private:
		SDL_Surface* m_bg;
		unsigned int m_ltime;

		// Les bonus
		struct InterBonus
		{
			int idx; // L'index dans le tableau des bonus
			unsigned int timeleft;
			unsigned int x, y;
		};
		typedef std::list<InterBonus>::iterator bon_it;
		typedef std::list<InterBonus>::const_iterator cbon_it;
		std::list<InterBonus> m_bonus;

		void deleteBonus(bon_it it);
		void addBonus(unsigned int x, unsigned int y, unsigned int bon);

		// Les tiles
		enum Status{
			SNK0,
			SNK1,
			SNK2,
			SNK3,
			NOTHING
		};
		std::list<std::pair<unsigned int,unsigned int>> m_status;
		void deleteStatus(unsigned int x, unsigned int y);

		struct Tile
		{
			ColType col;
			Status state;
			bon_it bon; // Valide si col == BONUS
		};
		Tile m_map[widthMap][heightMap];

};

#endif

