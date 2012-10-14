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
#include "BYGameLayer.h"


class BYGameScene : public cocos2d::CCLayer, public b2ContactListener {
private:
    
    typedef enum {
        GUI_Incorrect,
        GUI_PauseMenu,
        GUI_PauseMenu_Continue,
        GUI_GameFinishMenu,
        GUI_GoalLabel /// vicroty use this too
    } BYGUIElements;
    
    typedef CCLayer super;
    
    /// graphics background, used for world loading
    const float            boardShortStickLength();
    
    /// GUI:
    /// load score labels and pause button
    void loadUI();
    cocos2d::CCLabelTTF *_labelTopPlayerGoalsScored; /// weak ref
    cocos2d::CCLabelTTF *_labelBotPlayerGoalsScored; /// weak ref
    void pauseButtonHandler(cocos2d::CCMenuItem* menuItem); /// will show PauseMenu
    /// has 2 buttons: restart / quit. Restart: this->restartGame().
    cocos2d::CCMenu*    createPauseMenu(); /// creates autorelease object
    /// has 2 btns: re / quit, modifies menu from createPauseMenu
    void presentGameFinishMenu();
    void quitGame();
    void pauseGame();
    void continueGame();
    void finishGame();  /// called from CCSequence in playerDidScore, not to be called mannualy
    cocos2d::CCLabelTTF*            createGoalLabelForPlayer(BYGamePlayer player);
    cocos2d::CCFiniteTimeAction*    createGoalLabelSpawn(); /// goal and victory animations
    
    
    /// GameLogic
    void resetGame(); /// calls resetScore(), resetWorldObjects()
    
    
    /// score handle
    void resetScore();
    int _topPlayerScore;
    int _botPlayerScore;
    
    
    BYGameLayer*        m_gameLayer;
public:
    static cocos2d::CCScene* scene();
    BYGameScene();
    ~BYGameScene();
        
    /// contact listener
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
    
    /// will increase _playerScore, restart game, show victory if needed
    void playerDidScore(BYGamePlayer player);
    const cocos2d::CCPoint boardCornerPoint();
};

#endif /* defined(__TouchHockey__BYGameScene__) */
