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

BYBall:: ~BYBall() {
    
}

void BYBall::initPhysics(b2World* openWorld) {
    b2BodyDef bodyDef;
    bodyDef.type     = b2_dynamicBody;
    bodyDef.position = vecFromPoint(_bodySprite->getPosition());
    bodyDef.userData = _bodySprite;
    bodyDef.bullet   = true;
    bodyDef.fixedRotation = true;
    
    _bodyBox = openWorld->CreateBody(& bodyDef);

    b2CircleShape circleShape;
    circleShape.m_radius = _bodySprite->getContentSize().width / 2 / PTM_RATIO;

    b2FixtureDef fixtureDef;
    fixtureDef.shape    = &circleShape;
    fixtureDef.density  = 0.9f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 1.0f;

    _bodyBox->CreateFixture(&fixtureDef);
}


const b2Vec2 BYBall::getLinearVelocity() {
    return _bodyBox->GetLinearVelocity();
}


void BYBall::resetForces() {
    _bodyBox->SetLinearVelocity(b2Vec2_zero);
}