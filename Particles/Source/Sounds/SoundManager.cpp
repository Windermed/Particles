#include "SoundManager.h"
#include "Constants.h"
#include <iostream>
#include <filesystem>

using namespace std;

SoundManager::SoundManager()
{
	// automatically preload all avaliable sounds into memory
	for (auto& entry : filesystem::directory_iterator("content/sounds"))
	{
		string ext = entry.path().extension().string();
		// check for wav ogg flac audio files.
		if (ext == ".wav" || ext == ".ogg" || ext == ".flac")
		{
			string fullFilePath = entry.path().string();
			string fileName = entry.path().filename().string();

			// skips if it already loaded
			if (m_buffers.find(fileName) != m_buffers.end())
				continue;

			SoundBuffer buffer;
			
			if (!buffer.loadFromFile(fullFilePath))
			{
				Message("ERROR! Failed to load sound: " << fileName << "\n")
				Message("Ensure that the audio file you loaded is valid or exists!")
				continue;
			}
			m_buffers[fileName] = buffer;
			m_sounds[fileName].setBuffer(m_buffers[fileName]);

		}
	}
}
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