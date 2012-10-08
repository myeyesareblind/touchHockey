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


class BYGameScene : public cocos2d::CCLayer, public b2ContactListener {
private:
    
    typedef CCLayer super;
    
    /// graphics background, used for world loading
    const cocos2d::CCPoint boardCornerPoint();
    const float            boardShortStickLength();
    
    
    /// GUI:
    void pauseButtonHandler(cocos2d::CCMenuItem* menuItem);
    void loadUI();
    cocos2d::CCLabelTTF *_labelTopPlayerGoalsScored; /// weak ref
    cocos2d::CCLabelTTF *_labelBotPlayerGoalsScored; /// weak ref
    
    /// GameLogic
    void delegateTouchesToPaddles(cocos2d::CCSet* tSet);
    void loadBoxWorld();
    b2World  *_world; /// strong ref, deleted in ~BYGameScene()
    
    /// weak-references, owner - this->child array
    BYPaddle *_topPaddle;
    BYPaddle *_botPaddle;
    BYBall   *_ball;
    
public:
    static cocos2d::CCScene* scene();
    BYGameScene();
    ~BYGameScene();
    
    /// cclayer
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void update(float dt);
    
    /// contact listener
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif /* defined(__TouchHockey__BYGameScene__) */
