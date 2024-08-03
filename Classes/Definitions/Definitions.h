#pragma once
#include "cocos2d.h"
#include "PathDefinition.h"

USING_NS_CC;

#define VISIBLE_SIZE         Director::getInstance()->getVisibleSize()
#define VISIBLE_SIZE_WIDTH   Director::getInstance()->getVisibleSize().width
#define VISIBLE_SIZE_HEIGHT  Director::getInstance()->getVisibleSize().height

#define TYPE_WALL   1	

#define TYPE_HERO    2		
#define TYPE_ENEMY 4		
#define TYPE_NPC	 8		

#define TYPE_ITEM   16		
#define TYPE_COIN	 32		
#define TYPE_WEAPON  64		
#define TYPE_ARTICLE	 128

#define TRANSITION_TIME 0.5

#define ORDER_BACKGROUND_LAYER      1
#define ORDER_PLAYER                10
#define ORDER_MIDDLEGROUND_LAYER    20
#define ORDER_HUB_LAYER             100

#define TAG_ACTION_MOVE     1
#define TAG_ACTION_IDLE     2
#define TAG_ACTION_ATTACK   3

#define PTM_RATIO 32

static Color3B WHITE = Color3B(255, 255, 255);
static Color3B BROWN1 = Color3B(145, 50, 0);
static Color3B BROWN2 = Color3B(100, 34, 0);
static Color3B BROWN3 = Color3B(60, 20, 20);
static Color3B DARK_BLU = Color3B(0, 25, 25);
//
static Color3B BORDER_YELLOW = Color3B(206, 158, 56);
static Color3B BORDER_ORANGE = Color3B(231, 79, 0);
static Color3B BORDER_BROWN = Color3B(185, 169, 145);
//
static Color3B TEXT_YELLOW = Color3B(237, 224, 23);
static Color3B TEXT_BLUE = Color3B(62, 240, 227);


#define CHARACTER_COLLISION          0b000000001
#define MONSTER_COLLISION            0b000010010
#define BULLET_CHARACTER_COLLISION   0b000000100
#define BULLET_MONSTER_COLLISION     0b000001000
#define PILLAR_COLLISION             0b000100000

#define CHARACTER_CATEGORY           0b000001000
#define MONSTER_CATEGORY             0b000100100
#define BULLET_CHARACTER_CATEGORY    0b000000010
#define BULLET_MONSTER_CATEGORY      0b000000001
#define PILLAR_CATEGORY              0b000010000

#define CHARACTER_CONTACT            0b000000001
#define MONSTER_CONTACT              0b000010010
#define BULLET_CHARACTER_CONTACT     0b000000100
#define BULLET_MONSTER_CONTACT       0b000001000
#define PILLAR_CONTACT               0b000100000


#define CREATE_SET(varType, varName, funName)\
protected: varType varName;public: virtual inline void set##funName(varType var){ varName = var; }


#define CREATE_FUNC1(__TYPE__, __TYPE__PARAM_1) \
static __TYPE__* create(__TYPE__PARAM_1 var) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init(var)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

#define CREATE_FUNC_RETAIN(__TYPE__) \
static __TYPE__* create( ) \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->retain(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}