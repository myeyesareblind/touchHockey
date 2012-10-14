//
//  ConstantsAndMacros.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef TouchHockey_ConstantsAndMacros_h
#define TouchHockey_ConstantsAndMacros_h

#define PTM_RATIO 32
#define vecFromPoint(__ccpoint__) b2Vec2(__ccpoint__.x / PTM_RATIO, __ccpoint__.y / PTM_RATIO)
#define pointFromVec(__b2vec__) CCPointMake(__b2vec__.x * PTM_RATIO, __b2vec__.y * PTM_RATIO)

#define BYGoalsPerGame 7

#define CCDirectorManager  CCDirector::sharedDirector()
#endif
