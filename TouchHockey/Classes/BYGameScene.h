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
    
    typedef enum {
        BYGamePlayer_incorrect,
        BYGamePlayer_topPlayer,
        BYGamePlayer_botPlayer,
    } BYGamePlayer;
    
    typedef enum {
        GUI_PauseMenu
    } BYGUIElements;
    
    typedef CCLayer super;
    
    /// graphics background, used for world loading
    const cocos2d::CCPoint boardCornerPoint();
    const float            boardShortStickLength();
    
    /// GUI:
    /// load score labels and pause button
    void loadUI();
    cocos2d::CCLabelTTF *_labelTopPlayerGoalsScored; /// weak ref
    cocos2d::CCLabelTTF *_labelBotPlayerGoalsScored; /// weak ref
    void pauseButtonHandler(cocos2d::CCMenuItem* menuItem); /// will show PauseMenu
    /// has 2 buttons: restart / quit. Restart: this->restartGame().
    cocos2d::CCMenu*    createPauseMenu(); /// creates autorelease object
    void quitGame();
    void continueGame();
    
    /// GameLogic
    void resetGame(); /// calls resetScore(), resetWorldObjects()
    /// pass touches to BYPaddles
    void delegateTouchesToPaddles(cocos2d::CCSet* tSet);
    /// init box2dWorld
    void loadBoxWorld();
    /// set start positions of game objects, reset score labels
    
    b2World  *_world; /// strong ref, deleted in ~BYGameScene()

    /// weak-references, owner - this->child array
    void resetGameObjects(); /// set positions, remove forces from ball and paddle mousejoints
    BYPaddle *_topPaddle;
    BYPaddle *_botPaddle;
    BYBall   *_ball;
    
    cocos2d::CCRect _playArea;
    /// will increase _playerScore, restart game, show victory if needed
    void checkIfBallScored(const cocos2d::CCPoint& ballPosition);
    
    /// score handle
    /// dont see any reason to create spec class
    void resetScore();
    int _topPlayerScore;
    int _botPlayerScore;
    
    void showVictoryForPlayer(BYGamePlayer player);
    void showGoalForPlayer(BYGamePlayer player);
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
