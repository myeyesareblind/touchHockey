//
//  GameObjectDefinitions.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/15/12.
//
//

#include "cocos2d.h"

#ifndef TouchHockey_GameObjectDefinitions_h
#define TouchHockey_GameObjectDefinitions_h


typedef struct CCLine {

    CCLine(cocos2d::CCPoint pointA, cocos2d::CCPoint pointB):
    pointStart(pointA), pointFinish(pointB) {}
    
    float yValueAtX(float x) {
        /// line equation Ax+By+C = 0, y = (-Ax - C) / B
        float A = pointStart.y - pointFinish.y;
        float B = pointFinish.x - pointStart.x;
        float C = pointStart.x * pointFinish.y - pointFinish.x * pointStart.y;
        
        return (-A * x  - C) / B;
    }

    cocos2d::CCPoint pointStart;
    cocos2d::CCPoint pointFinish;
} CCLine;




/// the actuall force is multiplayed by b2body.Mass
typedef enum {
    
    BYMouseJointForce_Invalid = 0,
    BYMouseJointForce_Small   = 20,
    BYMouseJointForce_Middle  = 800,
    BYMouseJointForce_High    = 2000, /// BYPaddle mouse joint init
    BYMouseJointForce_Max     = 4000
    
} BYMouseJointForce;


/// this value is multiplayed by _bodySprite.Radius
typedef enum {
    
    BYAIAtackRadius_Invalid = 0,
    BYAIAtackRadius_Small   = 2,
    BYAIAtackRadius_Middle  = 3,
    BYAIAtackRadius_High    = 5,
    BYAIAtackRadius_Max     = 6
    
} BYAIAtackRadius;


typedef struct BYAIDifficulty {
    
    BYAIDifficulty(float rad, float mj, float mjAtack) :
    atackRadius(rad),
    mouseJointForceFollow(mj),
    mouseJointForceAtack(mjAtack)
    {
    };
    
    float atackRadius;
    
    float mouseJointForceFollow;
    
    float mouseJointForceAtack;
    
} BYAIDifficulty;


inline BYAIDifficulty* choboAI() {
    static BYAIDifficulty dif(BYAIAtackRadius_Small,
                              BYMouseJointForce_Small,
                              BYMouseJointForce_Middle);
    return &dif;
}


inline BYAIDifficulty* hasuAI() {
    static BYAIDifficulty dif(BYAIAtackRadius_Middle,
                              BYMouseJointForce_Middle,
                              BYMouseJointForce_High);
    return &dif;
}

inline BYAIDifficulty* gosuAI() {
    static BYAIDifficulty dif(BYAIAtackRadius_High,
                              BYMouseJointForce_High,
                              BYMouseJointForce_Max);
    return &dif;
}

#endif