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

using namespace cocos2d;


BYPaddle :: BYPaddle() {
}

BYPaddle::~BYPaddle() {
}

void BYPaddle::initPhysics(b2World* openWorld) {
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
    fixtureDef.density  = 1.0f;
    fixtureDef.friction = 0.99f;
    fixtureDef.restitution = 0.1f;
    
    _bodyBox->CreateFixture(&fixtureDef);
    
    /// hack, finding ground box
    _world = openWorld;
    _groundBody = _world->GetBodyList();
    /// groundBody is the only body, which doesnt have sprite attached
    while (_groundBody) {
        if (NULL == _groundBody->GetUserData()) {
            break;
        }
        _groundBody = _groundBody->GetNext();
    }
    
    b2MouseJointDef mouseJointDef;
    mouseJointDef.bodyA = _groundBody;
    mouseJointDef.bodyB = _bodyBox;
    mouseJointDef.collideConnected = true;
    mouseJointDef.maxForce         = 2000.0f * _bodyBox->GetMass();
    ///! it wont work without these
    mouseJointDef.target           = _bodyBox->GetPosition();
    
    _mouseJoint = (b2MouseJoint *) _world->CreateJoint(&mouseJointDef);
}


const b2Vec2  BYPaddle::getLinearVelocity() {
    return b2Vec2(0,0);
}



void BYPaddle::setMoveArea(const CCRect& moveArea) {
    float radius = _bodySprite->getContentSize().width / 2;
    
    _minPositionPoint = CCPointMake(moveArea.origin.x + radius,
                                    moveArea.origin.y + radius);
    
    _maxPositionPoint = CCPointMake(moveArea.origin.x + moveArea.size.width - radius,
                                    moveArea.origin.y + moveArea.size.height - radius);
}


void BYPaddle::handleTouchAtPoint(const CCPoint& pnt) {
//    
//    CCPoint retPoint(pnt);
//    /// trim max values
//    float y = min(retPoint.y, _maxPositionPoint.y);
//    float x = min(retPoint.x, _maxPositionPoint.x);
//    
//    /// trim min values
//    y = max(y, _minPositionPoint.y);
//    x = max(x, _minPositionPoint.x);
//    
//    retPoint.x = x;
//    retPoint.y = y;
//    this->jumpToPoint(retPoint);
    this->jumpToPoint(pnt);
}


void BYPaddle::jumpToPoint(const CCPoint& jumpPoint) {
    CCLog("paddle about to jump");
    
    _mouseJoint->SetTarget(vecFromPoint(jumpPoint));
}


void BYPaddle::setPosition(const cocos2d::CCPoint& point) {
    super::setPosition(point);
    
    /// set position, then jump
    _bodyBox->SetTransform(vecFromPoint(point), 0);
    this->jumpToPoint(point);
}



