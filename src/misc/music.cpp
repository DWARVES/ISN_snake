
#include "music.hpp"
#include "config.hpp"
#include <sstream>

Music* music;

Music::Music()
	: m_mus(NULL)
{
	// Global
	music = this;

	for(int i = 0; i < NONE; ++i)
		m_snds[i] = NULL;

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 1024) < 0)
		return;

	// Chargement musique
	std::ostringstream path;
	path << rcdir << music_subdir << "mus.ogg";
	m_mus = Mix_LoadMUS(path.str().c_str());
	Mix_VolumeMusic(128);

	// Chargement sons
	if(Mix_AllocateChannels(3) != 3)
		return;
	for(int i = 0; i < 3; ++i)
		Mix_Volume(i, 128);

	path.str("");
	path << rcdir << music_subdir << "ok.wav";
	m_snds[0] = Mix_LoadWAV(path.str().c_str());
	path.str("");
	path << rcdir << music_subdir << "cancel.wav";
	m_snds[1] = Mix_LoadWAV(path.str().c_str());
	path.str("");
	path << rcdir << music_subdir << "death.wav";
	m_snds[2] = Mix_LoadWAV(path.str().c_str());
	path.str("");
	path << rcdir << music_subdir << "eat.wav";
	m_snds[3] = Mix_LoadWAV(path.str().c_str());
}

Music::~Music()
{
	if(m_mus != NULL)
		Mix_FreeMusic(m_mus);

	for(int i = 0; i < NONE; ++i)
	{
		if(m_snds[i] != NULL)
			Mix_FreeChunk(m_snds[i]);
	}

	Mix_CloseAudio();
	music = NULL;
}

void Music::playMusic()
{
	if(m_mus != NULL)
		Mix_PlayMusic(m_mus, -1);
}

void Music::stopMusic()
{
	Mix_HaltMusic();
}

void Music::playSound(Snd id)
{
	if(m_snds[id] != NULL)
		Mix_PlayChannel(-1, m_snds[id], 0);
}


