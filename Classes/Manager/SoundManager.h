

#pragma once
#include "cocos2d.h"
#include "AudioEngine.h"
USING_NS_CC;
class SoundManager
{
public:
	static SoundManager* getInstance();
	void setBackgroundMusicVolume(float volume);
	void setEffectVolume(float volume);
	void playBackgroundMusic(std::string filePath);
	void playEffect(std::string filePath);
	void stopBackgroundMusic();
	void stopAll();
	void setPlay(bool value);
	bool getPlay();
private:
	SoundManager();
	static SoundManager* instance;
	float backgroundMusicVolume;
	float effectVolume;
	int backgroundMusicId;
	int effectId;
	bool soundOn;
};

