//
//  BYBall.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYBall.h"
#include "Box2D.h"
#include "ConstantsAndMacros.h"
#include "BYBoxWorld.h"

BYBall:: ~BYBall() {
    
}

void BYBall::initPhysics() {
    b2BodyDef bodyDef;
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = vecFromPoint(_bodySprite->getPosition());
    bodyDef.userData = _bodySprite;
    bodyDef.bullet   = true;
	bodyDef.angularDamping = 0.9f;

    b2World *openWorld = BYBoxWorld::BoxWorldInstance().getWorld();
    _bodyBox = openWorld->CreateBody(& bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = _bodySprite->getContentSize().width / 2 / PTM_RATIO;

    b2FixtureDef fixtureDef;
    fixtureDef.shape    = &circleShape;
    fixtureDef.density  = 0.8f;
    fixtureDef.friction = 0.7f;
    fixtureDef.restitution = 0.3f;

    _bodyBox->CreateFixture(&fixtureDef);
}