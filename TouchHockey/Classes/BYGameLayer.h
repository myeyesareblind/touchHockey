//
//  BYGameLayer.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/14/12.
//
//

#ifndef __TouchHockey__BYGameLayer__
#define __TouchHockey__BYGameLayer__


#include <iostream>
#include "cocos2d.h"
#include "BYPlayerPaddle.h"
#include "BYBall.h"
#include "BYAIPaddle.h"

typedef enum {
    BYGamePlayer_incorrect,
    BYGamePlayer_topPlayer,
    BYGamePlayer_botPlayer,
} BYGamePlayer;


/// forward declaration
class BYGameScene;


class BYGameLayer : public cocos2d::CCLayer{
    
    typedef enum {
        GameMode_Multiplayer,
        GameMode_SinglePlayer
    } GameMode;
    
    GameMode m_GameMode;
    
    typedef CCLayer super;
    
    
    void    loadBoxWorld();
    
    
    /// Game Objects, strong refs
    BYAIPaddle*           m_aiPaddle;     /// replaces top player
    BYPlayerPaddle*       m_topPaddle;    /// = NULL for single player
    BYPlayerPaddle*       m_botPaddle;
    BYBall*               m_ball;


    b2World*        m_world; /// strong ref, deleted in ~BYGameScene()
    
    
    BYGameScene*    m_GameLogic;
    
    
    void update(float dt);
    
    
    void checkIfBallScored(const cocos2d::CCPoint& ballPosition);
    

    /// pass touches to BYPaddles
    void delegateTouchesToPaddles(cocos2d::CCSet* tSet);
    
    
public:
    
    BYGameLayer();
    ~BYGameLayer();
    
#if COCOS2D_DEBUG > 0

    virtual void draw();
    
#endif
    
    
    void    pauseWorld();
    
    
    void    resumeWorld();
    
    
    bool    initWithMultiPlayer();


    bool    initWithSinglePlayer();
    
    
    void    setScene(BYGameScene* scene);
    
    /// set positions, remove forces from ball and paddle mousejoints
    void    resetGameObjects();
    
    /// cclayer
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
};


inline void BYGameLayer::setScene(BYGameScene* scene) {
    CCAssert(scene, "Cant set NULL scene");
    
    m_GameLogic  = scene;
}

#endif /* defined(__TouchHockey__BYGameLayer__) */