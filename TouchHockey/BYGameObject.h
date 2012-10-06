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

class BYGameObject : public cocos2d::CCObject {
    cocos2d::CCSprite* _bodySprite;
    /// boxBody
    
public:
    BYGameObject();
    virtual ~BYGameObject(void);
    bool init(cocos2d::CCString* spriteFileName);
    
    cocos2d::CCSprite* getSprite(void);
    void setCenterAtPosition(const cocos2d::CCPoint& point);
};

#endif /* defined(__TouchHockey__BYGameObject__) */
