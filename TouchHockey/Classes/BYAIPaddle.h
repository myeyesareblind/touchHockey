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
#include "BYPaddle.h"
#include "GameObjectDefinitions.h"
#include "StateMachine.h"
#include "BYAIStates.h"

class BYBall;

class BYAIPaddle : public BYPaddle {

protected:
    
    BYBall*             m_ball;             /// weak pointer
    
    BYAIDifficulty*     m_difficulty;       /// weak pointer
    
    StateMachine<BYAIPaddle>*   m_stateMachine;
    
    friend  BYAIStateFollowBall;
    
public:
    
    
    BYAIPaddle():   m_ball(NULL),
                    m_difficulty(NULL),
                    m_stateMachine(NULL)
    {};
    
    
    virtual ~BYAIPaddle() {
        
        delete m_stateMachine;
        
    };
    
    
    void update(float d);
    
    
    /// setters
    void setBall(BYBall* ball) {
        if (ball) {
            m_ball = ball;
        }
    }
    
    
    void setDifficulty(BYAIDifficulty* difficulty) {
        if (difficulty) {
            m_difficulty = difficulty;
        }
    };
    
    
    /// getters
    BYBall* getBall() {
        return m_ball;
    }
    
    float getAtackRadius() {
        
        return m_difficulty->atackRadius * _bodySprite->getContentSize().width / 2;
    }
    
    StateMachine<BYAIPaddle>* getFSM() {
        return m_stateMachine;
    }
    
};


#endif /* defined(__TouchHockey__BYAIPaddle__) */
