//
//  BYPaddle.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYPaddle__
#define __TouchHockey__BYPaddle__

#include <iostream>
#include "BYGameObject.h"
#include "Box2D.h"

class BYPaddle : public BYGameObject {
    
protected:
    cocos2d::CCPoint _maxPositionPoint;
    cocos2d::CCPoint _minPositionPoint;

    
    /// weak pointers, created in BYGameScene
    b2Body          *_groundBody;
    
    /// for moving body
    b2MouseJoint    *_mouseJoint;
    
    void jumpToPoint(const cocos2d::CCPoint& point);
    
    virtual void initPhysics(b2World* world);
    
    typedef BYGameObject super;
    
public:
    BYPaddle();
    virtual ~BYPaddle();
    
    virtual const b2Vec2 getLinearVelocity();
        
    /// rect will be trimmed to count radius
    void setMoveArea(const cocos2d::CCRect& rect);
    
    /// override since mouseJoint here
    virtual void setPosition(const cocos2d::CCPoint& point);
};

#endif /* defined(__TouchHockey__BYPaddle__) */