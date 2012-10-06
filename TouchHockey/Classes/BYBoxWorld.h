//
//  BYBoxWorld.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYBoxWorld__
#define __TouchHockey__BYBoxWorld__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

/*
 box made as singleton, cuz boxBodys are created in BYGameObject
 gets deleted in BYGameScene destructor
*/


#pragma mark - FIX_ME, i will crush on second run

class BYBoxWorld : public cocos2d::CCObject {
    
public:
    static BYBoxWorld& BoxWorldInstance() {
        static BYBoxWorld singleton;
        return singleton;
    }
    
    b2World* getWorld();
    
private:
    b2World *_world;
    
    BYBoxWorld();
    ~BYBoxWorld();
    BYBoxWorld(const BYBoxWorld& root);
    const BYBoxWorld& operator=(BYBoxWorld&);
};

#endif /* defined(__TouchHockey__BYBoxWorld__) */