//
//  BYGameScene.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYGameScene.h"
#include "BYPaddle.h"
#include "ConstantsAndMacros.h"
//#include "GLES-Render.h"

using namespace cocos2d;


const float BYGoalLabelAnimetionInterval = 1.0f;
const float BYGoalLabelFreezeAnimationInterval = 0.5f;
const float BYGoalLabelScaleBy           = 5;
const float BYGoalLabelRotatesCount      = 2;


const CCPoint BYGameScene::boardCornerPoint() {
    return CCPointMake(70, 24);
}


CCScene* BYGameScene::scene() {
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new BYGameScene();
    scene->addChild(layer);
    layer->autorelease();
    
    return scene;
}


BYGameScene::BYGameScene() {
    CCSize    winSize = CCDirectorManager->getWinSize();
    
    /// get Atlas
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("game.plist", "game.png");
    
    /// add background sprite
    CCSprite *background = CCSprite::createWithSpriteFrameName("background.png");
    background->cocos2d::CCNode::setPosition(winSize.width /2, winSize.height / 2);
    this->addChild(background);
    this->setContentSize(background->getContentSize());
    
    m_gameLayer = new BYGameLayer();
    
    m_gameLayer->initWithMultiPlayer();
    m_gameLayer->setScene(this);

    this->addChild(m_gameLayer);
    m_gameLayer->autorelease();
    
    this->loadUI();
    
    this->resetScore();
}



BYGameScene::~BYGameScene() {
}


#pragma mark - GamePlay methods

void BYGameScene::resetGame() {
    
    m_gameLayer->resetGameObjects();
    
    this->resetScore();
    
    this->continueGame();
}


void BYGameScene::resetScore() {
    _topPlayerScore = _botPlayerScore = 0;
    
    CCString* str = CCString::createWithFormat("%d", 0);
    _labelBotPlayerGoalsScored->setString(str->getCString());
    _labelTopPlayerGoalsScored->setString(str->getCString());    
}


void BYGameScene::finishGame() {
    
    this->removeChildByTag(GUI_GoalLabel, true);
    /// whos winner?
    /// a bit tricky. I dont own the winner
    BYGamePlayer gameWinner = _topPlayerScore > _botPlayerScore ? BYGamePlayer_topPlayer : BYGamePlayer_botPlayer;
    
    /// show victory label + animation
    CCLabelTTF *labelVictory = this->createGoalLabelForPlayer(gameWinner);
    labelVictory->setString("Victory!");
    this->addChild(labelVictory);
    CCCallFunc *presentFinishMenu = CCCallFunc::create(this,
                                               callfunc_selector(BYGameScene::presentGameFinishMenu));
    CCFiniteTimeAction *action = CCSequence::create(this->createGoalLabelSpawn(),
                                                    presentFinishMenu,
                                                    NULL
                                                    );
    labelVictory->runAction(action);
}


void BYGameScene::playerDidScore(BYGamePlayer player) {
    CCAssert(player, "Incorrect player scored");
    
    this->pauseGame();
    
    int             *playerGoals;
    CCLabelTTF      *scoreLabel        = NULL;
    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (player == BYGamePlayer_topPlayer) {
        playerGoals = & _topPlayerScore;
        scoreLabel  = _labelTopPlayerGoalsScored;
    } else {
        playerGoals  = & _botPlayerScore;
        scoreLabel        = _labelBotPlayerGoalsScored;
    }
    
    /// update score label
    (*playerGoals) ++;
    CCString* str = CCString::createWithFormat("%d", *playerGoals);
    scoreLabel->setString( str->getCString());
    
    /// show goal animations: scale, rotate, fadein
    CCLabelTTF *labelGoal = this->createGoalLabelForPlayer(player);
    labelGoal->setString("Goal!");
    this->addChild(labelGoal);
    
    CCFiniteTimeAction *action;
    if (*playerGoals == BYGoalsPerGame) {
        
        /// On animation finish I show nextAnimation on new label
        /// that animation triggers finish game menu pop-up
        CCCallFunc *finishGameFunc = CCCallFunc::create(this,
                                                        callfunc_selector(BYGameScene::finishGame));
        action = CCSequence::create(this->createGoalLabelSpawn(),
                                    finishGameFunc,
                                    NULL);
    } else {
        
        CCCallFunc *resetFunc = CCCallFunc::create(m_gameLayer,
                                                   callfunc_selector(BYGameLayer::resetGameObjects));
        CCCallFunc *contFunc  = CCCallFunc::create(this,
                                                   callfunc_selector(BYGameScene::continueGame));
        
        action =  CCSequence::create(this->createGoalLabelSpawn(),
                                     resetFunc,
                                     contFunc,
                                     NULL);

    }
    labelGoal->runAction(action);
}


void BYGameScene::presentGameFinishMenu() {
    this->removeChildByTag(GUI_GoalLabel, true);
    
    CCMenu *menu = this->createPauseMenu();
    menu->removeChildByTag(GUI_PauseMenu_Continue, true);
    
    this->addChild(menu);
}


CCLabelTTF* BYGameScene::createGoalLabelForPlayer(BYGamePlayer player) {
    CCPoint         pos;
    float           rotation           = 0;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    pos.x = winSize.width / 2;
    if (player == BYGamePlayer_topPlayer) {
        pos.y = winSize.height * 3.0f / 4;
        rotation    = 180;
    } else {
        pos.y  = winSize.height / 4;
    }
    
    
    CCLabelTTF* lbl = CCLabelTTF::create("",
                                         "Marker Felt",
                                         40);
    lbl->setOpacity(0);
    lbl->setScale(BYGoalLabelScaleBy);
    lbl->setTag(GUI_GoalLabel);
    lbl->setPosition(pos);
    lbl->setRotation(rotation);
    
    return lbl;
}


CCFiniteTimeAction* BYGameScene::createGoalLabelSpawn() {
    
    CCFadeIn *fadeIn = CCFadeIn::create(255);
    fadeIn->setDuration(BYGoalLabelAnimetionInterval);
    
    CCRotateBy *rotBY = CCRotateBy::create(BYGoalLabelFreezeAnimationInterval,
                                           360 * BYGoalLabelRotatesCount);
    
    CCScaleBy *scaleBy = CCScaleBy::create(BYGoalLabelFreezeAnimationInterval,
                                           1 / BYGoalLabelScaleBy);
    
    return CCSpawn::create(fadeIn, rotBY, scaleBy, NULL);
}


#pragma mark - GUI methods

void BYGameScene::loadUI() {
    
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("ui.plist", "ui.png");
    
    CCSize winSize    = CCDirector::sharedDirector()->getWinSize();    
    
    /// pause button
    CCSprite* btnNormalSprite = CCSprite::createWithSpriteFrameName("pause.png");
    CCMenuItemSprite *pauseMenuItem = CCMenuItemSprite::create(btnNormalSprite,
                                                               btnNormalSprite,
                                                               this,
                                                               menu_selector(BYGameScene::pauseButtonHandler));
    
    /// 2 labels for goals
    ccColor3B color = ccc3(220, 70, 20); /// red color
    _labelBotPlayerGoalsScored = CCLabelTTF::create("0", "Marker Felt", 40);
    _labelBotPlayerGoalsScored->setColor(color);
    CCMenuItemLabel *labelBot = CCMenuItemLabel::create(_labelBotPlayerGoalsScored, NULL, NULL);
    labelBot->setEnabled(false);
    
    _labelTopPlayerGoalsScored = CCLabelTTF::create("0", "Marker Felt", 40);
    _labelTopPlayerGoalsScored->setColor(color);
    CCMenuItemLabel *labelTop = CCMenuItemLabel::create(_labelTopPlayerGoalsScored, NULL, NULL);
    labelTop->setEnabled(false);
    
    /// CCMenu doesnt support anchorPoint as of 0x00020003 version
    CCMenu *menu = CCMenu::create(labelTop, pauseMenuItem, labelBot, NULL);
    menu->alignItemsHorizontallyWithPadding(5);
    menu->setAnchorPoint(CCPointMake(0, 0));
    menu->setPosition(CCPointMake(winSize.width - pauseMenuItem->getContentSize().width / 2,
                                  winSize.height / 2));
    menu->setRotation(90);
    this->addChild(menu);
}


void BYGameScene::pauseButtonHandler(CCMenuItem* pauseItem) {
    CCLog("game paused");
    CCMenu* pauseMenu = this->createPauseMenu();
    this->addChild(pauseMenu);
    
    this->pauseGame();
}


cocos2d::CCMenu*  BYGameScene::createPauseMenu() { /// creates autorelease object
    CCMenuItemFont* continueItem = CCMenuItemFont::create("Continue",
                                                          this,
                                                          menu_selector(BYGameScene::continueGame));
    continueItem->setTag(GUI_PauseMenu_Continue);
    
    CCMenuItemFont* reItem = CCMenuItemFont::create("Restart",
                                                    this,
                                                    menu_selector(BYGameScene::resetGame));
    CCMenuItemFont* quitItem = CCMenuItemFont::create("Quit",
                                                      this,
                                                      menu_selector(BYGameScene::quitGame));
    
    CCMenu* menu = CCMenu::create(continueItem, reItem, quitItem, NULL);
    menu->alignItemsVerticallyWithPadding(5);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    menu->setPosition(CCPointMake(s.width / 2, s.height / 2));
    menu->setTag(GUI_PauseMenu);
    
    return menu;
}



void BYGameScene::quitGame() {
    CCDirector::sharedDirector()->popScene();
}


void BYGameScene::pauseGame() {
    
    m_gameLayer->pauseWorld();
    
}


void BYGameScene::continueGame() {
    
    m_gameLayer->resumeWorld();
    
    this->removeChildByTag(GUI_PauseMenu, true);
    this->removeChildByTag(GUI_GoalLabel, true);
}


#pragma mark - b2ContactListener

void BYGameScene::BeginContact(b2Contact* contact) {
//    CCLog("contact began");
//    b2Fixture* fixtureA = contact->GetFixtureA();
//    b2Fixture* fixtureB = contact->GetFixtureB();
//    
//    b2Body* bodyA = fixtureA->GetBody();
//    b2Body* bodyB = fixtureB->GetBody();
//    
//    CCObject* objA = (CCObject*) bodyA->GetUserData();
//    CCObject* objB = (CCObject*) bodyB->GetUserData();
//    
//    /// return if not ball
//    if (objA != _ball
//        || objB != _ball) {
//        return ;
//    }
//    CCLog("ball contacted");
}


void BYGameScene::EndContact(b2Contact* contact) {
    
}


void BYGameScene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    
}

void BYGameScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    CCLog("PostSolve contact");
}