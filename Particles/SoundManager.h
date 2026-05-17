#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

using namespace sf;
using namespace std;


class SoundManager
{
public:
	static SoundManager& GetInstance()
	{
		static SoundManager instance;
		return instance;
	}

	void PlaySound(const string& fileName);
private:
	SoundManager() {}

	map<string, SoundBuffer> m_buffers;
	map<string, Sound> m_sounds;
};

