//
//  BYBall.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYBall__
#define __TouchHockey__BYBall__

#include <iostream>
#include "BYGameObject.h"

class BYBall : public BYGameObject {

protected:
    typedef BYGameObject super;
    virtual void initPhysics(b2World* world);
    
public:
    virtual ~BYBall();
    virtual const b2Vec2 getLinearVelocity();
    void    resetForces();
};

#endif /* defined(__TouchHockey__BYBall__) */
