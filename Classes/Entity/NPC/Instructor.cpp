#include "Instructor.h"
#include "Manager/GameManager.h"
#include "Entity/Entities.h"
#include "Game/Quest.h"
#include "Database/DataManager.h"

bool Instructor::init()
{
	if (!NPC::init()) return false;
	_effect = Sprite::createWithSpriteFrameName("question_mark1");
	_effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_effect, 2);
	this->scheduleUpdate();
	return true;
}

void Instructor::setName(std::string name)
{
	NPC::setName(name);
	_effect->setPosition(_contentSize.width / 2, _contentSize.height + 70);
}

void Instructor::onClick()
{
	auto cha = GameManager::getInstance()->getCharacter();
	auto quest = cha->getQuest();
	// not complete
	if (quest->getStatus() == Quest::UNFINISHED) {
		auto dialog = Dialog::create("You quickly complete your mission!");
		GameManager::getInstance()->getHUDLayer()->addChild(dialog, 1000);
		return;
	}

	std::vector<std::string> conversations;
	int index = 0;
	std::function<void()> callback = nullptr;

	// unrecevided
	if (quest->getStatus() == Quest::UNRECEIVED) {
		conversations = quest->getConversation1();
		callback = [=]() {
			quest->setStatus(Quest::Status::UNFINISHED);
		};

	}
	// completed
	else if (quest->getStatus() == Quest::COMPLETE) {
		conversations = quest->getConversation2();
		callback = [&, cha]() {
			auto currenQuest = cha->getQuest();
			auto qm = QuestManager::create();
			auto q = qm->getQuest(currenQuest->getId() + 1);
			qm->updateQuest(q);
			cha->setQuest(q);
			auto knowMap = CharacterManager::create()->getKnowMaps();
			cha->setKnowMaps(knowMap);
			cha->setCurrentExp(cha->getCurrentExp() + currenQuest->getExp(), true);
			cha->setCoin(cha->getCoin() + currenQuest->getCoin(), true);
			CC_SAFE_DELETE(currenQuest);
		};
	}
	showConversations(conversations, callback);
}

void Instructor::showConversations(std::vector<std::string> conversations, std::function<void()> callback)
{
	auto hudLayer = GameManager::getInstance()->getHUDLayer();
	Vector<Dialog*> dialogs;

	for (int index = 0; index < conversations.size(); index++)
	{
		auto dialog = Dialog::create(conversations.at(index));
		dialogs.pushBack(dialog);
		hudLayer->addChild(dialog, 1000 - index);
		if (index != 0)
			dialog->setVisible(false);
	}
	for (int i = 0; i < dialogs.size() - 1; i++)
	{
		dialogs.at(i)->setCloseCallback([=] {
			dialogs.at(i + 1)->setVisible(true);
			});
	}
	dialogs.at(dialogs.size() - 1)->setCloseCallback(callback);
}

void Instructor::update(float dt)
{
	auto cha = GameManager::getInstance()->getCharacter();
	auto status = cha->getQuest()->getStatus();

	auto camPos = Director::getInstance()->getRunningScene()->getDefaultCamera()->getPosition();
	auto visibleRect = Rect(camPos.x - VISIBLE_SIZE_WIDTH / 2,
		camPos.y - VISIBLE_SIZE_HEIGHT / 2, VISIBLE_SIZE_WIDTH, VISIBLE_SIZE_HEIGHT);

	auto hudLayer = GameManager::getInstance()->getHUDLayer();
	Sprite* arow = (Sprite*)hudLayer->getChildByName("Direction");
	if (!visibleRect.containsPoint(_position) && status != Quest::Status::UNFINISHED) {
		if (!arow) {
			auto sprite = Sprite::create(PATH_IMAGE_UI + "arow.png");
			sprite->setName("Direction");
			arow = sprite;
			auto action = Utility::createAnimationWaggle(10);
			arow->runAction(action);
			hudLayer->addChild(arow);
			if (cha->getPosition().x < _position.x) {
				arow->setFlippedX(false);
				arow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
				arow->setPosition(Vec2(hudLayer->getContentSize().width * 0.98, hudLayer->getContentSize().height * 0.6));
			}
			else {
				arow->setFlippedX(true);
				arow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				arow->setPosition(Vec2(hudLayer->getContentSize().width * 0.02, hudLayer->getContentSize().height * 0.6));
			}
		}
	}
	else {
		if (arow) {
			arow->removeFromParent();
		}
	}


	if (status == Quest::Status::UNFINISHED) {
		_effect->setVisible(false);
		_effect->stopAllActions();
		return;
	}

	Animation* ani;
	_effect->setVisible(true);
	if (status == Quest::Status::UNRECEIVED) {
		ani = Utility::createAnimation("question_mark", 0.2, -1);
		if (!_effect->getActionByTag(1)) {
			_effect->stopAllActions();
			auto action = Animate::create(ani);
			action->setTag(1);
			_effect->runAction(action);
		}
	}
	else if (status == Quest::Status::COMPLETE) {
		ani = Utility::createAnimation("exclamation_mark", 0.2, -1);
		if (!_effect->getActionByTag(2)) {
			_effect->stopAllActions();
			auto action = Animate::create(ani);
			action->setTag(2);
			_effect->runAction(action);
		}
	}

}
