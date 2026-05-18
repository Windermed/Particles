#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>


using namespace sf;
using namespace std;


class SoundManager
{
public:
	SoundManager();

	static SoundManager& GetInstance() // in the future, we should add a SoundManager to Engine that can be accessed.
	{
		static SoundManager instance;
		return instance;
	}

	void PlaySound(const string& fileName, float volume = 100.0f, bool bLoop = false);
	void PlaySoundPooled(const string& fileName, float volume = 100.0f);
	void StopSound(const string& fileName);
	void StopAll(); // stop all audio.
private:
	string ResolvePath(const string& fileName);
private:

	map<string, SoundBuffer> m_buffers;
	map<string, Sound> m_sounds;

	map<string, vector<Sound>> m_soundPool;
	map<string, int> m_poolIndex;
};

