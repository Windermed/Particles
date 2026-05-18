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
void SoundManager::PlaySound(const string& fileName, float volume, bool bLoop)
{

	// let's resolve the actual path first.
	string resolvedAudioPath = ResolvePath(fileName);

	// let's load our audio.
	if (m_buffers.find(resolvedAudioPath) == m_buffers.end())
	{
		SoundBuffer buffer;
		if (!buffer.loadFromFile(resolvedAudioPath))
		{
			Message("ERROR! Failed to load sound: " << resolvedAudioPath << "\n")
			Message("Ensure that the audio file you loaded is valid or exists!")
			return;
		}
		m_buffers[resolvedAudioPath] = buffer;
		m_sounds[resolvedAudioPath].setBuffer(m_buffers[resolvedAudioPath]);
	}

	// we'll allow loops assuming bLoop is set to true.
	m_sounds[resolvedAudioPath].setVolume(volume);
	m_sounds[resolvedAudioPath].setLoop(bLoop);
	m_sounds[resolvedAudioPath].play();
}

void SoundManager::PlaySoundPooled(const string& fileName, float volume)
{
	string resolvedFile = ResolvePath(fileName);

	// load buffer if needed.
	if (m_buffers.find(resolvedFile) == m_buffers.end())
	{
		SoundBuffer buffer;
		if (!buffer.loadFromFile(resolvedFile))
		{
			Message("PlaySoundPooled: FAILED TO LOAD: " << resolvedFile);
			Message("Ensure that the file is valid and try again!");
			return;
		}
		m_buffers[resolvedFile] = buffer;
	}

	// creates a pool of 8 instances if one does not exist.
	if (m_soundPool.find(resolvedFile) == m_soundPool.end())
	{
		m_soundPool[resolvedFile] = vector<Sound>(8);
		for (Sound& s : m_soundPool[resolvedFile])
		{
			s.setBuffer(m_buffers[resolvedFile]);
		}
		m_poolIndex[resolvedFile] = 0;
	}

	// get the next avaliabel sound in pool
	int& index = m_poolIndex[resolvedFile];
	Sound& sound = m_soundPool[resolvedFile][index];

	sound.setVolume(volume);
	sound.play();

	// advance to the next slot.
	index = (index + 1) % m_soundPool[resolvedFile].size();
}

void SoundManager::StopSound(const string& fileName)
{
	// let's resolve the actual path first.
	string resolvedAudioPath = ResolvePath(fileName);

	auto it = m_sounds.find(fileName);
	if (it != m_sounds.end())
	{
		it->second.stop();
	}
}

void SoundManager::StopAll()
{
	for (auto& pair : m_sounds)
		pair.second.stop();
}

string SoundManager::ResolvePath(const string& fileName)
{
	// if we get the full path
	if (fileName.find("content/") != string::npos)
		return fileName;

	// if we just get the file name or subdirectory.
	// such as "snd_fortressman.wav" or "content/sounds/limbo/snd_crystal_whoosh.wav"
	// then we'll just resolve to content/sounds/.
	return "content/sounds/" + fileName;
}