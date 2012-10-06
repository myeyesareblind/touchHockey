//
//  BYGameObject.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYGameObject.h"

using namespace cocos2d;

BYGameObject::BYGameObject() {
}


BYGameObject::~BYGameObject() {
    _bodySprite->release();
}



bool BYGameObject::init(cocos2d::CCString* spriteFileName) {
    CCAssert(spriteFileName->length(), "Cant init paddle with null-sprite");
    _bodySprite = CCSprite::createWithSpriteFrameName(spriteFileName->getCString());
    _bodySprite->retain();
    _bodySprite->setAnchorPoint(CCPointMake(0, 0));
    
    return true;
}


void BYGameObject::setCenterAtPosition(const cocos2d::CCPoint& cPoint) {
    
    /// move box2d
    
    /// move sprite
    float halfPaddleWidth = _bodySprite->getContentSize().width / 2;
    CCPoint paddleCenterOrigin = CCPointMake(cPoint.x -
                                             halfPaddleWidth,
                                             cPoint.y - halfPaddleWidth);
    
    _bodySprite->setPosition(paddleCenterOrigin);
}


cocos2d::CCSprite* BYGameObject::getSprite(void) {
    return _bodySprite;
}

bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
}

void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
}

void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
}

void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
}