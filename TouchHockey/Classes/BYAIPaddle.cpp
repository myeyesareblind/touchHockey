//
//  BYAIPaddle.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/15/12.
//
//

#include "BYAIPaddle.h"
#include "BYBall.h"

using namespace cocos2d;


void BYAIPaddle::update(float d) {
    if (! m_stateMachine) {
        /// cant put this in constructor since I m using init
        m_stateMachine = new StateMachine<BYAIPaddle>(this);
        m_stateMachine->SetCurrentState(BYAIStateFollowBall::Instance());
    }
    m_stateMachine->Update();
}