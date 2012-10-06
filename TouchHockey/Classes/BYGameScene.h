//
//  BYGameScene.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYGameScene__
#define __TouchHockey__BYGameScene__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"
#include "BYPaddle.h"
#include "BYBall.h"


class BYGameScene : public cocos2d::CCLayer {
private:
    b2World  *_world;
    
    /// weak-references, owner - this->child array
    BYPaddle *_topPaddle;
    BYPaddle *_botPaddle;
    BYBall   *_ball;
    
    cocos2d::CCRect   _paddleTopArea;
    cocos2d::CCRect   _paddleBotArea;
    cocos2d::CCRect   _playArea;
    
    typedef CCLayer super;
    
    void loadBoxWorld();
    
    /// graphics background, used for world loading
    const cocos2d::CCPoint boardCornerPoint();
    const float            boardShortStickLength();
    
public:
    static cocos2d::CCScene* scene();
    BYGameScene();
    ~BYGameScene();
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void update(float dt);
};

#endif /* defined(__TouchHockey__BYGameScene__) */
