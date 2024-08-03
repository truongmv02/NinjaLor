#include "HUDLayer.h"
#include "Manager/GameManager.h"
#include "ControlLayer.h"
#include "Lib/CardInput.h"
#include "InventoryLayer.h"
#include "EquipmentLayer.h"
#include "Lib/CardInput.h"

bool HUDLayer::init() {
	if (!Layer::init()) {
		return false;
	}
	_charStateBoard = CharacterStateBoard::create(GameManager::getInstance()->getCharacter());
	this->addChild(_charStateBoard);
	_eStateBoard = EnemyStateBoard::create();
	this->addChild(_eStateBoard, 0);

	_controlLayer = ControlLayer::create();
	this->addChild(_controlLayer, 1);

	return true;
}

void HUDLayer::addMessage(Message* message, int length)
{
	auto currentMessage = (Message*)this->getChildByName("Message");
	if (currentMessage) {
		if (currentMessage->getMessage() == message->getMessage()) {
			message->release();
			return;
		}

		if (length != 0) {
			auto message1 = currentMessage->getMessage();
			auto message2 = message->getMessage();
			message1.erase(message1.begin() + length, message1.end());
			message2.erase(message2.begin() + length, message2.end());

			if (message1 == message2) {
				auto timeCounter = this->getChildByName("TimeCounter");
				if (timeCounter) {
					timeCounter->unscheduleUpdate();
					timeCounter->removeFromParent();
					currentMessage->removeFromParent();
				}
			}
		}
	}

	auto  timeCounter = CallbackTimeCounter::create();
	timeCounter->setName("TimeCounter");
	message->setName("Message");
	this->addChild(timeCounter);
	timeCounter->initWithCondition([=]() {
		this->addChild(message);
		message->run();
		timeCounter->unscheduleUpdate();
		timeCounter->removeFromParent();
		}, [=]() {
			if (!this->getChildByName("Message")) return true;
			return false;
		});
}
