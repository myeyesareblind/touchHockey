//
//  BYAIPaddle.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/15/12.
//
//

#ifndef __TouchHockey__BYAIPaddle__
#define __TouchHockey__BYAIPaddle__

#include <iostream>
#include "BYGameObject.h"


class BYAIPaddle : public BYGameObject {
    
    
public:
    
    
    BYAIPaddle();
    
    virtual const b2Vec2 getLinearVelocity();
    
    
    
};

#endif /* defined(__TouchHockey__BYAIPaddle__) */
