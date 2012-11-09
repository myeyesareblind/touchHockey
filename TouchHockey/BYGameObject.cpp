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



bool BYGameObject::init(cocos2d::CCString* spriteFileName,
                        b2World* world,
                        const CCPoint& position) {
    
    CCAssert(spriteFileName->length(), "Cant init paddle with null-sprite");
    _bodySprite = CCSprite::createWithSpriteFrameName(spriteFileName->getCString());
    _bodySprite->retain();
    _bodySprite->setPosition(position);
    CCAssert(_bodySprite, "Cant init gameObject bodySprite");
    
    initPhysics(world);
    
    return true;
}



void BYGameObject::initPhysics(b2World* world) {
    CCAssert(0x00, "Subclass should overwrite this method");
}



void BYGameObject::setPosition(const cocos2d::CCPoint& point) {
    /// move sprite
    _bodySprite->setPosition(point);
    
    /// move box2d
    _bodyBox->SetTransform(vecFromPoint(point), 0);
}



cocos2d::CCSprite* BYGameObject::getSprite(void) {
    return _bodySprite;
}


cocos2d::CCPoint BYGameObject::getLinearVelocity() {
    b2Vec2 vec = _bodyBox->GetLinearVelocity();
    return pointFromVec(vec);
}

