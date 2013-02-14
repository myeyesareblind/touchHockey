//
//  BYMainMenuScene.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYMainMenuScene.h"
#include "BYGameScene.h"

using namespace cocos2d;


CCScene* BYMainMenuLayer::scene() {
    CCScene *scene = CCScene::create();
    
    CCLayer *layer = new BYMainMenuLayer();
    layer->autorelease();
    scene->addChild(layer);

    return scene;
}


BYMainMenuLayer::BYMainMenuLayer() {
#pragma TODO: add background image
    /* create CCMenuItems:
        1 Plyaer
        2 Players
        3 Settings
    */
    
    CCMenuItemFont* singlePlayerItem = CCMenuItemFont::create("Single Player",
                                                              this,
                                                              menu_selector(BYMainMenuLayer::MenuButton_SinglePlayerHandle));
    
    CCMenuItemFont* twoPlayersItem = CCMenuItemFont::create("Two Players",
                                                              this,
                                                              menu_selector(BYMainMenuLayer::MenuButton_TwoPlayersHandle));
    
    CCMenuItemFont* settingsItem = CCMenuItemFont::create("Single Player",
                                                              this,
                                                              menu_selector(BYMainMenuLayer::MenuButton_SettingsHandle));
    /// create menu node
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCMenu *menu = CCMenu::create(singlePlayerItem, twoPlayersItem, settingsItem, NULL);
    menu->setPosition(s.width / 2, s.height / 2);
    this->addChild(menu);
    menu->alignItemsVerticallyWithPadding(50);
}


BYMainMenuLayer::~BYMainMenuLayer() {
    CCLOG("MainMenu destructor");
}


void BYMainMenuLayer::MenuButton_SinglePlayerHandle(CCMenuItem* item) {
    CCDirector::sharedDirector()->pushScene(BYGameScene::singlePlayerScene());
}

void BYMainMenuLayer::MenuButton_TwoPlayersHandle(CCMenuItem* item) {
    CCDirector::sharedDirector()->pushScene(BYGameScene::multiPlayerScene());
}

void BYMainMenuLayer::MenuButton_SettingsHandle(CCMenuItem* itemClicked) {
    CCAssert(0, "To be implemented");
}