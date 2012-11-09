//
//  BYAIStates.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/20/12.
//
//

#ifndef __TouchHockey__BYAIStates__
#define __TouchHockey__BYAIStates__

#include <iostream>
#include "State.h"


class BYAIPaddle;


class BYAIStateFollowBall : public State<BYAIPaddle> {
    
private:
    
    BYAIStateFollowBall(){}
    
    BYAIStateFollowBall(const BYAIStateFollowBall&);
    BYAIStateFollowBall& operator=(const BYAIStateFollowBall&);
    
public:
    
    static BYAIStateFollowBall* Instance();
    
    virtual void Enter(BYAIPaddle* paddle);
    
    virtual void Execute(BYAIPaddle* paddle);
    
    virtual void Exit(BYAIPaddle* paddle);
    
//    virtual bool OnMessage(Miner* agent, const Telegram& msg);
};



class BYAIStateAtack {
    
private:
    
    BYAIStateAtack(){}
    BYAIStateAtack(const BYAIStateAtack&);
    BYAIStateAtack& operator=(const BYAIStateAtack&);
    
public:
    
    static BYAIStateAtack* Instance();
    
    virtual void Enter(BYAIPaddle* paddle);
    
    virtual void Execute(BYAIPaddle* pad);
    
    virtual void Exit(BYAIPaddle* paddle);
    
};


class BYAIStateDefend {
    
private:
    
    BYAIStateDefend(){}
    BYAIStateDefend(const BYAIStateDefend&);
    BYAIStateDefend& operator=(const BYAIStateDefend&);
    
public:
    
    static BYAIStateDefend* Instance();
    
    virtual void Enter(BYAIStateDefend* paddle);
    
    virtual void Execute(BYAIStateDefend* pad);
    
    virtual void Exit(BYAIStateDefend* paddle);
    
};


#endif /* defined(__TouchHockey__BYAIStates__) */
