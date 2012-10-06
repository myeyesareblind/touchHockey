//
//  BYMainMenuScene.h
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#ifndef __TouchHockey__BYMainMenuScene__
#define __TouchHockey__BYMainMenuScene__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

class BYMainMenuLayer : public cocos2d::CCLayer {
    
public:
    BYMainMenuLayer();
    ~BYMainMenuLayer();
    static cocos2d::CCScene * scene();
    
private:
    void MenuButton_SinglePlayerHandle(cocos2d::CCMenuItem* item);
    void MenuButton_TwoPlayersHandle(cocos2d::CCMenuItem* item);
    void MenuButton_SettingsHandle(cocos2d::CCMenuItem* item);
};

#endif /* defined(__TouchHockey__BYMainMenuScene__) */
