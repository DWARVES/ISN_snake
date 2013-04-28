
#ifndef DEF_MUSIC
#define DEF_MUSIC

#include <SDL_mixer.h>

class Music
{
	public:
		enum Snd{OK, CANCEL, DEATH, EAT, NONE};

		Music();
		~Music();

		void playMusic();
		void stopMusic();

		void playSound(Snd id);

	private:
		Mix_Music* m_mus;
		Mix_Chunk* m_snds[NONE];
};
extern Music* music;

#endif

