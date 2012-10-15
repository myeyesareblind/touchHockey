//
//  BYPlayerPaddle.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/15/12.
//
//

#include "BYPlayerPaddle.h"
#include "ConstantsAndMacros.h"

using namespace cocos2d;

void BYPlayerPaddle::handleTouchAtPoint(const CCPoint& pnt) {
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


void BYPlayerPaddle::jumpToPoint(const CCPoint& jumpPoint) {
    CCLog("paddle about to jump");
    
    _mouseJoint->SetTarget(vecFromPoint(jumpPoint));
}