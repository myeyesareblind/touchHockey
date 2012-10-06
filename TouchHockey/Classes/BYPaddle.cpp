//
//  BYPaddle.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYPaddle.h"
#include "Box2D.h"
#include "ConstantsAndMacros.h"
#include "BYBoxWorld.h"

using namespace cocos2d;

BYPaddle :: BYPaddle() {
    
}

BYPaddle::~BYPaddle() {
    
}

void BYPaddle::initPhysics() {
    b2BodyDef bodyDef;
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = vecFromPoint(_bodySprite->getPosition());
    bodyDef.userData = _bodySprite;
    bodyDef.bullet   = true;
    
    b2World *openWorld = BYBoxWorld::BoxWorldInstance().getWorld();
    _bodyBox = openWorld->CreateBody(& bodyDef);
    
    b2CircleShape circleShape;
    circleShape.m_radius = _bodySprite->getContentSize().width / 2 / PTM_RATIO;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape    = &circleShape;
    fixtureDef.density  = 1.0f;
    fixtureDef.friction = 0.99f;
    fixtureDef.restitution = 0.1f;
    
    _bodyBox->CreateFixture(&fixtureDef);
}