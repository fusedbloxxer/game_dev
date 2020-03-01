#include "stdafx.h"
#include "Sound.h"

Sound::Sound(const std::shared_ptr<SoundResource>& soundResource)
	:soundResource{ soundResource }, sound{ nullptr }, channel{ nullptr }, playFunc{ nullptr } {}

Sound* Sound::init(const std::shared_ptr<SoundResource>& soundResource)
{
	if (holdsResources)
	{
		freeResources();
	}

	this->soundResource = soundResource;

	return this;
}

void Sound::setUpSystem()
{
	// Create FMOD System.
	succeedOrThrow(FMOD::System_Create(&fmodSystem), "FMOD: System could not be created");

	// Initialize FMOD.
	succeedOrThrow(fmodSystem->init(512, FMOD_INIT_NORMAL, NULL), "FMOD: System could not be initialized.");

	// Create channel group.
	succeedOrThrow(fmodSystem->createChannelGroup("InGameSoundEffects", &channelGroup), "FMOD: Could not create in-game sound effects channel group");

}

void Sound::freeResources()
{
	if (holdsResources)
	{
		if (sound != nullptr)
		{
			succeedOrThrow(sound->release(), "FMOD: Could not release FMOD::Sound* resource.");
		}

		if (channelGroup != nullptr)
		{
			// succeedOrThrow(channelGroup->release(), "FMOD: Could not release FMOD::ChannelGroup* resource.");
		}

		if (fmodSystem != nullptr)
		{
			// succeedOrThrow(fmodSystem->release(), "FMOD: Could not release FMOD::System* resource.");
		}

		holdsResources = false;
	}
}

void Sound::load()
{
	load(FMOD_LOOP_OFF, false);
}

void Sound::load(const FMOD_MODE isLooping, const bool isOverlapping)
{
	playFunc = isOverlapping ? &Sound::playSoundWithOverlap : &Sound::playSoundWithoutOverlap;
	succeedOrThrow(fmodSystem->createSound(soundResource->file.c_str(), isLooping, 0, &(this->sound)), "FMOD: Could not load sound into memory.");
	holdsResources = true;
}

void Sound::playSound()
{
	(this->*playFunc)();
}

const bool Sound::isPlaying()
{
	if (channel != nullptr)
	{
		bool isPlaying = false;
		succeedOrThrow(channel->isPlaying(&isPlaying), "FMOD: Could not obtain sound state.");
		succeedOrThrow(fmodSystem->update(), "FMOD: Could not update system state.");
		return isPlaying;
	}
	return false;
}

void Sound::playSoundWithOverlap()
{
	succeedOrThrow(fmodSystem->playSound(sound, 0, false, 0), "FMOD: Sound cannot be played.");
}

void Sound::playSoundWithoutOverlap()
{
	if (!isPlaying())
	{
		succeedOrThrow(fmodSystem->playSound(sound, 0, false, &channel), "FMOD: Sound cannot be played.");
		succeedOrThrow(channel->setChannelGroup(channelGroup), "FMOD: Failed to set channel group.");
	}
}

bool Sound::succeedOrThrow(const FMOD_RESULT result, const std::string& message)
{
	if (result != FMOD_OK)
	{
		// !TODO;
		// throw std::runtime_error{ message };
		return false;
	}
	return true;
}

Sound::~Sound()
{
	freeResources();
}
