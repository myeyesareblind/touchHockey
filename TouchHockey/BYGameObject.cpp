//
//  BYGameObject.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYGameObject.h"
#include "ConstantsAndMacros.h"
#include "BYBoxWorld.h"

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
    CCAssert(_bodySprite, "Cant init gameObject bodySprite");
    
    initPhysics();
    
    return true;
}


void BYGameObject::initPhysics() {
    CCAssert(0x00, "Subclass should overwrite this method");
}


bool BYGameObject::containsPoint(CCPoint& pnt) {
    const CCPoint& origin  = _bodySprite->getPosition();
    const CCSize&  size    = _bodySprite->getContentSize();
    CCRect thisRect = CCRectMake(origin.x,
                                 origin.y,
                                 size.width,
                                 size.height);
    
    return thisRect.containsPoint(pnt);
}


void BYGameObject::setCenterAtPosition(const cocos2d::CCPoint& cPoint) {
    
    /// move sprite
    float halfPaddleWidth = _bodySprite->getContentSize().width / 2;
    CCPoint paddleCenterOrigin = CCPointMake(cPoint.x -
                                             halfPaddleWidth,
                                             cPoint.y - halfPaddleWidth);
    
    _bodySprite->setPosition(paddleCenterOrigin);
    
    /// move box2d
    _bodyBox->SetTransform(vecFromPoint(paddleCenterOrigin), 0);
}



cocos2d::CCSprite* BYGameObject::getSprite(void) {
    return _bodySprite;
}