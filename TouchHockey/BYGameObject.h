//
//  BYGameObject.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYGameObject__
#define __TouchHockey__BYGameObject__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"


/// root class for BYPaddle and BYBall
class BYGameObject : public cocos2d::CCObject {
    
protected:
    
    cocos2d::CCSprite* _bodySprite;
    b2Body*            _bodyBox;    // strong ref
    b2World*           _world;      // weak ref
    
    /// creates b2body etc
    virtual void initPhysics(b2World *world);
    
public:
    
    BYGameObject();
    virtual ~BYGameObject(void);
    
    /// creates _bodySprite, calls initPhysics
    bool    init(cocos2d::CCString* spriteFileName,
                 b2World *world,
                 const cocos2d::CCPoint& position);
        
    /// getters
    cocos2d::CCSprite* getSprite(void);
    b2Body*            getWorldBody();
    
    /// will set sprite / body at that point
    virtual void setPosition(const cocos2d::CCPoint& point);
    
    /// return a copy, as does box2d
    virtual const b2Vec2 getLinearVelocity() = 0;
};


inline b2Body* BYGameObject::getWorldBody()
{
    return _bodyBox;
}


#endif /* defined(__TouchHockey__BYGameObject__) */