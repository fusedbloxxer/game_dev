#pragma once
#include "Loadable.h"
#include "SoundResource.h"
#include "..\Utilities\fmod-2.00.07\fmod.hpp"

class Sound : public Loadable
{
	inline static FMOD::ChannelGroup* channelGroup = nullptr;

	inline static FMOD::System* fmodSystem = nullptr;

	std::shared_ptr<SoundResource> soundResource;

	FMOD::Channel* channel;

	FMOD::Sound* sound;

	void (Sound::*playFunc)();

public:
	Sound(const std::shared_ptr<SoundResource>& soundResource);

	Sound* init(const std::shared_ptr<SoundResource>& soundResource);

	static void setUpSystem();

	// Inherited via Loadable
	virtual void freeResources() override;

	void load(const FMOD_MODE sound, const bool isOverlapping);

	virtual void load() override;

	const bool isPlaying();

	void playSound();

	virtual ~Sound();

private:
	void playSoundWithoutOverlap();

	void playSoundWithOverlap();

private:
	static bool succeedOrThrow(const FMOD_RESULT result, const std::string& message);
};

