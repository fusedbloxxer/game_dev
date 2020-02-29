#include "stdafx.h"
#include "Sound.h"

Sound::Sound(std::shared_ptr<SoundResource> soundResource)
	:soundResource{ soundResource }, sound{ nullptr }, isPlaying{ false }, channelGroup{ nullptr }, channel{ nullptr } {}

Sound& Sound::init(std::shared_ptr<SoundResource> soundResource)
{
	if (holdsResources)
	{
		freeResources();
	}

	// Create FMOD System.
	succeedOrThrow(FMOD::System_Create(&fmodSystem), "FMOD: System could not be created");

	// Initialize FMOD.
	succeedOrThrow(fmodSystem->init(512, FMOD_INIT_NORMAL, NULL), "FMOD: System could not be initialized.");
	
	// Create channel group.
	succeedOrThrow(fmodSystem->createChannelGroup("InGameSoundEffects", &channelGroup), "FMOD: Could not create in-game sound effects channel group");

	this->soundResource = soundResource;
	holdsResources = true;

	return *this;
}

void Sound::freeResources()
{
	if (holdsResources)
	{
		if (sound != nullptr)
		{
			succeedOrThrow(sound->release(), "FMOD: Could not release FMOD::Sound* resource.");
		}

		if (fmodSystem != nullptr)
		{
			succeedOrThrow(fmodSystem->release(), "FMOD: Could not release FMOD::System* resource.");
		}

		if (channelGroup != nullptr)
		{
			succeedOrThrow(channelGroup->release(), "FMOD: Could not release FMOD::ChannelGroup* resource.");
		}

		holdsResources = false;
	}
}

void Sound::load()
{
	load(FMOD_LOOP_OFF);
}

void Sound::load(const FMOD_MODE sound)
{
	succeedOrThrow(fmodSystem->createSound(soundResource->file.c_str(), sound, 0, &(this->sound)), "FMOD: Could not load sound into memory.");
}

void Sound::playSound()
{
	succeedOrThrow(fmodSystem->playSound(sound, 0, false, 0), "FMOD: Sound cannot be played.");
}

void Sound::playSoundWithoutOverlap()
{
	succeedOrThrow(fmodSystem->update(), "FMOD: Could not update system state.");

	if (!channel || (succeedOrThrow(channel->isPlaying(&isPlaying), "FMOD: Could not obtain sound state.") && !isPlaying))
	{
		succeedOrThrow(fmodSystem->playSound(sound, 0, false, &channel), "FMOD: Sound cannot be played.");
		succeedOrThrow(channel->setChannelGroup(channelGroup), "FMOD: Failed to set channel group.");
	}
}

bool Sound::succeedOrThrow(const FMOD_RESULT result, const std::string& message)
{
	if (result != FMOD_OK)
	{
		throw std::runtime_error{ message };
		return false;
	}
	return true;
}

Sound::~Sound()
{
	freeResources();
}
