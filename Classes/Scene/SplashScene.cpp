#include "SplashScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "Definitions/Definitions.h"
#include "Database/SkillManager.h"
#include "Lib/Lib.h"
bool SplashScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	_total = 6;
	_count = 0;

	sliderLoading = Slider::create();
	sliderLoading->loadBarTexture(PATH_IMAGE_UI + "loading_bg.png");
	sliderLoading->loadProgressBarTexture(PATH_IMAGE_UI + "loading_progress.png");
	sliderLoading->setEnabled(false);
	sliderLoading->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 5));
	sliderLoading->setPercent(0);
	this->addChild(sliderLoading);
	loadResources();
	this->scheduleUpdate();
	return true;
}


void SplashScene::loadResources() {

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_EFFECT + "effect_skill.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_EFFECT + "effect.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_EFFECT + "damage_effect.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_ITEM + "equipment.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_MONSTER + "monsters.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_UI + "pointer.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(PATH_IMAGE_ITEM + "item_icon.plist");

	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_EFFECT + "effect_skill.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_EFFECT + "effect.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_EFFECT + "damage_effect.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_ITEM + "equipment.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_MONSTER + "monsters.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_UI + "pointer.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync(PATH_IMAGE_ITEM + "item_icon.png", CC_CALLBACK_1(SplashScene::loadingCallBack, this));
}

void SplashScene::loadingCallBack(Texture2D* texture) {
	_count++;
	float percent = (float)_count / (float)_total * 100;
	sliderLoading->setPercent(percent);
}

void SplashScene::update(float dt) {
	if (_count >= _total) {
		this->unscheduleUpdate();
		auto scene = MainMenuScene::create();
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}
}