#include "SoundManager.h"
#include "Constants.h"
#include <iostream>
using namespace std;

void SoundManager::PlaySound(const string& fileName)
{
	// let's load the audio
	if (m_buffers.find(fileName) == m_buffers.end())
	{
		SoundBuffer buffer;
		if (!buffer.loadFromFile(fileName))
		{
			Message("ERROR! Failed to load sound: " << fileName << "\n")
			Message("Ensure that the audio file you loaded is valid or exists!")
			return;
		}
		m_buffers[fileName] = buffer;
		m_sounds[fileName].setBuffer(m_buffers[fileName]);	
	}

	m_sounds[fileName].play();
}