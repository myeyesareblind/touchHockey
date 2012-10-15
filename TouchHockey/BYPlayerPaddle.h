//
//  BYPlayerPaddle.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/15/12.
//
//

#ifndef __TouchHockey__BYPlayerPaddle__
#define __TouchHockey__BYPlayerPaddle__

#include <iostream>
#include "BYPaddle.h"

class BYPlayerPaddle : public BYPaddle {
    
private:
    cocos2d::CCPoint _maxPositionPoint;
    cocos2d::CCPoint _minPositionPoint;
    
    void jumpToPoint(const cocos2d::CCPoint& point);
    
public:
    /// touches are passed here from BYGameObject.
    /// for any event, there will be 1 only touch passed for paddle
    /// touchEnded event is ignored
    /// will trim touches according to _max and _min positionPoint
    void handleTouchAtPoint(const cocos2d::CCPoint& pnt);
};

#endif /* defined(__TouchHockey__BYPlayerPaddle__) */
