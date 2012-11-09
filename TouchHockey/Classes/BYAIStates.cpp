//
//  BYAIStates.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/20/12.
//
//

#include "BYAIStates.h"
#include "BYAIPaddle.h"
#include "cocos2d.h"
#include "BYBall.h"



using namespace cocos2d;


#pragma mark BYAIStateFollowBall



BYAIStateFollowBall* BYAIStateFollowBall::Instance() {
    static BYAIStateFollowBall instance;
    return &instance;
}


void BYAIStateFollowBall::Enter(BYAIPaddle* pad) {
    pad->_mouseJoint->SetMaxForce(pad->m_difficulty->mouseJointForceFollow *
                                  pad->_bodyBox->GetMass());
    CCLOG("Entered followBallState");
}


void BYAIStateFollowBall::Execute(BYAIPaddle* pad) {
    
    BYBall  *ball     = pad->getBall();
    CCAssert(ball, "Ball should be setted");
    
    float   padRadius        = pad->getSprite()->getContentSize().width / 2;
    
    CCPoint ballPoint = ball->getSprite()->getPosition();
    CCPoint padPoint  = pad->getSprite()->getPosition();
    
    /// should atack state be triggered?
    /// do the trajectories collide?
    CCPoint ballVec    = ball->getLinearVelocity();
    CCLine  ballTraj(ballPoint,
                     CCPointMake(ballPoint.x + 1000 * ballVec.x ,
                                 ballPoint.y + 1000 * ballVec.y));
//
//    CCLine  atackTraj(padPositionPoint,
//                      CCPointMake(padPositionPoint.x,
//                                  padPositionPoint.y + pad->m_difficulty->atackRadius * padRadius));
//    
//    float xValue = atackTraj.pointStart.x;
//    float yValue = ballTraj.yValueAtX(xValue);
//    
//    if (yValue > atackTraj.pointStart.y && yValue < atackTraj.pointFinish.y) {
//        /// OK, trajectories do intersect
//        /// in what time ball will be at that point
//        float xDif = xValue - ballTraj.pointStart.x; /// distance
//        float time = xDif / ballVec.x;
//        
////        static float maxAtackTime = 
//    }
    
    
//    /// defend somehow
    if (ballPoint.y > padPoint.y && ballVec.y > 0) {
        CCLog("activating defend mode");
    }
//    /// if in atack range - perform atack
//    
//    
//    
//    int ballX = ballPoint.x;
//    int thisX = thisPoint.x;
//    
//    if ( ballX != thisX ) {
//        pad->jumpToPoint(CCPointMake(ballPoint.x, thisPoint.y));
//    }
}


void BYAIStateFollowBall::Exit(BYAIPaddle* pad) {
    CCLOG("switched from FollowBall State");
}



#pragma mark BYAIStateAtack

BYAIStateAtack* BYAIStateAtack::Instance() {
    static BYAIStateAtack instance;
    return &instance;
}


void BYAIStateAtack::Enter(BYAIPaddle* pad) {
    
}


void BYAIStateAtack::Execute(BYAIPaddle* pad) {
    
}


void BYAIStateAtack::Exit(BYAIPaddle* pad) {
    
}