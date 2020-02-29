#pragma once
#include "Loadable.h"
#include "SoundResource.h"
#include "..\Utilities\fmod-2.00.07\fmod.hpp"

class Sound : public Loadable
{
	inline static FMOD::System* fmodSystem = nullptr;

	std::shared_ptr<SoundResource> soundResource;

	FMOD::ChannelGroup* channelGroup;

	FMOD::Channel* channel;

	FMOD::Sound* sound;

	bool isPlaying;

public:
	Sound(std::shared_ptr<SoundResource> soundResource);

	Sound& init(std::shared_ptr<SoundResource> soundResource);

	// Inherited via Loadable
	virtual void freeResources() override;

	virtual void load() override;
	
	void load(const FMOD_MODE sound);

	void playSoundWithoutOverlap();

	void playSound();

	virtual ~Sound();

private:
	bool succeedOrThrow(const FMOD_RESULT result, const std::string& message);
};

