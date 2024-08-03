#include "SoundManager.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() {
	backgroundMusicVolume = 1.0f;
	effectVolume = 1.0f;
	soundOn = true;
}

SoundManager* SoundManager::getInstance() {
	if (instance == nullptr) {
		instance = new SoundManager();
	}
	return instance;
}

bool SoundManager::getPlay() {
	return soundOn;
}

void SoundManager::setPlay(bool value) {
	if (value)
		AudioEngine::resumeAll();
	else
		AudioEngine::pauseAll();
	soundOn = value;

}

void SoundManager::setBackgroundMusicVolume(float volume) {
	backgroundMusicVolume = volume;
	AudioEngine::setVolume(0, volume);
}

void SoundManager::setEffectVolume(float volume) {
	effectVolume = volume;
}

void SoundManager::playBackgroundMusic(std::string filePath) {
	if (soundOn)
		backgroundMusicId = AudioEngine::play2d(filePath, true, backgroundMusicVolume);
}

void SoundManager::playEffect(std::string filePath) {
	if (soundOn)
		AudioEngine::play2d(filePath, false, effectVolume);
}


void SoundManager::stopBackgroundMusic() {
	AudioEngine::stop(backgroundMusicId);
}

void SoundManager::stopAll() {
	AudioEngine::stopAll();
}
