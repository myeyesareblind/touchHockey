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


const float BYGameScene::boardShortStickLength() {
    return 184;
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
    
    this->setTouchEnabled(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    /// get Atlas
    CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    frameCache->addSpriteFramesWithFile("game.plist", "game.png");
    
    /// add background sprite
    CCSprite *background = CCSprite::createWithSpriteFrameName("background.png");
    background->cocos2d::CCNode::setPosition(winSize.width /2, winSize.height / 2);
#pragma mark - TODO: fix fps drop
    this->addChild(background);
    this->loadUI();
    
    this->loadBoxWorld();
    
    /// add players
    float quarterHeight = (float) winSize.height / 4;
    _topPaddle = new BYPaddle();
    _topPaddle->init(CCString::createWithFormat("paddle_yellow.png"),
                     _world,
                     CCPointMake(winSize.width / 2,  quarterHeight *3));
    this->addChild(_topPaddle->getSprite());
    
    _botPaddle = new BYPaddle();
    _botPaddle->init(CCString::createWithFormat("paddle_green.png"),
                     _world,
                     CCPointMake(winSize.width / 2, quarterHeight / 2));
    this->addChild(_botPaddle->getSprite());

    
    /// add ball
    _ball = new BYBall();
    _ball->init(CCString::createWithFormat("ball_blue.png"),
                _world,
                CCPointMake(winSize.width / 2, quarterHeight * 2));
    this->addChild(_ball->getSprite());
    
    CCPoint contPoint = boardCornerPoint();
    
    _playArea = CCRectMake(contPoint.x,
                           contPoint.y,
                           winSize.width  - 2 * contPoint.x,
                           winSize.height - 2 * contPoint.y);
    CCRect botRect(_playArea);
    botRect.size.height /= 2;
    _botPaddle->setMoveArea(botRect);
    
    CCRect topRect = CCRectMake(contPoint.x,
                                contPoint.y + _playArea.size.height / 2,
                                _playArea.size.width,
                                _playArea.size.height / 2);
    _topPaddle->setMoveArea(topRect);
    
    this->resetScore();
    
    this->scheduleUpdate();
}



BYGameScene::~BYGameScene() {
    delete _world;
    _topPaddle->release();
    _botPaddle->release();
    _ball->release();
}


#pragma mark - GamePlay methods

void BYGameScene::resetGame() {
    
    this->resetGameObjects();
    this->resetScore();
    
    this->continueGame();
}


void BYGameScene::resetScore() {
    _topPlayerScore = _botPlayerScore = 0;
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


void BYGameScene::checkIfBallScored(const CCPoint& ballPoint) {
    static float height = CCDirector::sharedDirector()->getWinSize().height;
    
    if (ballPoint.y < 0 || ballPoint.y > height ) {
        
        BYGamePlayer    playerScored = BYGamePlayer_incorrect;
        
        if (_playArea.origin.y > ballPoint.y) {
            playerScored = BYGamePlayer_topPlayer;
        } else {
            playerScored = BYGamePlayer_botPlayer;
        }
        this->playerDidScore(playerScored);
    }
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
        
        CCCallFunc *resetFunc = CCCallFunc::create(this,
                                                   callfunc_selector(BYGameScene::resetGameObjects));
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
    this->unscheduleUpdate();
    this->setTouchEnabled(false);
}


void BYGameScene::continueGame() {
    
    this->scheduleUpdate();
    this->setTouchEnabled(true);
    
    this->removeChildByTag(GUI_PauseMenu, true);
    this->removeChildByTag(GUI_GoalLabel, true);
}


#pragma mark - touch handle

void BYGameScene::ccTouchesBegan(cocos2d::CCSet *pTouches,
                                 cocos2d::CCEvent *pEvent) {
    
    this->delegateTouchesToPaddles(pTouches);
}


void BYGameScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    this->delegateTouchesToPaddles(pTouches);
}


void BYGameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    /// empty
}


void BYGameScene::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    /// empty
}


void BYGameScene::delegateTouchesToPaddles(CCSet* pTouches) {
    /*
     For single touch event process only single touch for each paddle.
     Any paddle can receive only 1 touch
     */
    
    bool passedToTopPaddle = false;
    bool passedToBotPaddle = false;
    static float halfY = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    CCSetIterator iterator;
    for(iterator = pTouches->begin();
        iterator != pTouches->end();
        iterator++)
    {
        CCTouch* touch = (CCTouch*)(*iterator);
        CCPoint touchPoint = touch->getLocation();
        
        CCPoint previousPoint = touch->getPreviousLocation();
        
        if (touchPoint.y > halfY) {
            if (! passedToTopPaddle) {
                _topPaddle->handleTouchAtPoint(touchPoint);
                passedToBotPaddle = true;
            }
        }
        
        else {
            if (! passedToBotPaddle) {
                _botPaddle->handleTouchAtPoint(touchPoint);
                passedToBotPaddle = true;
            }
            
        } /// else
    } /// iterator
}



#pragma mark - Box2D handle

void BYGameScene::loadBoxWorld() {
#warning TODO: check if bodys should be release
    b2Vec2 gravity = b2Vec2(0, 0);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(false);
    _world->SetContinuousPhysics(true);
    //    _world->SetContactListener(this);
    //    GLESDebugDraw*    m_debugDraw = new GLESDebugDraw( PTM_RATIO );
    //    _world->SetDebugDraw(m_debugDraw);
    //
    //    uint32 flags = 0;
    //    flags += b2Draw::e_shapeBit;
    //    flags += b2Draw::e_jointBit;
    //    flags += b2Draw::e_aabbBit;
    //    flags += b2Draw::e_pairBit;
    //    flags += b2Draw::e_centerOfMassBit;
    //    m_debugDraw->SetFlags(flags);
    
    
    /// add bounds
    CCPoint botLeftPoint = this->boardCornerPoint();
    float   boardShortStickLenght = boardShortStickLength();
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    b2Body *groundBody = _world->CreateBody(& groundBodyDef);
    b2EdgeShape groundBox;
    
    
    /// bottom left
    CCPoint botLeftPadPoint = CCPoint(botLeftPoint);
    botLeftPadPoint.x += boardShortStickLenght;
    groundBox.Set(vecFromPoint(botLeftPoint),
                  vecFromPoint(botLeftPadPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// left
    CCPoint topLeftPoint = CCPoint(botLeftPoint);
    topLeftPoint.y = s.height - botLeftPoint.y;
    groundBox.Set(vecFromPoint(botLeftPoint),
                  vecFromPoint(topLeftPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// top left
    CCPoint topLeftPadPoint = CCPoint(topLeftPoint);
    topLeftPadPoint.x += boardShortStickLenght;
    groundBox.Set(vecFromPoint(topLeftPoint),
                  vecFromPoint(topLeftPadPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    
    /// bot right
    CCPoint botRightPoint = CCPointMake(s.width - botLeftPoint.x, botLeftPoint.y);
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(CCPointMake(botRightPoint.x - boardShortStickLenght,
                                           botLeftPadPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// right
    CCPoint topRightPoint = CCPoint(botRightPoint);
    topRightPoint.y = s.height - botLeftPadPoint.y;
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(topRightPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// topRight
    groundBox.Set(vecFromPoint(topRightPoint),
                  vecFromPoint(CCPointMake(topRightPoint.x - boardShortStickLenght, topRightPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);
}


void BYGameScene::update(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    _world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    CCSprite *ballSprite = _ball->getSprite();
    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            b2Vec2 position = b->GetPosition();
            CCPoint point = CCPointMake( position.x * PTM_RATIO,
                                    position.y * PTM_RATIO);
            myActor->setPosition(point);
            
            /// if ball
            if (myActor == ballSprite) {
                this->checkIfBallScored(point);
            }
            
        }
    }
}


void BYGameScene::resetGameObjects() {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    float quarterHeight = (float) winSize.height / 4;
    _topPaddle->setPosition(CCPointMake(winSize.width / 2,  quarterHeight *3));
    _botPaddle->setPosition(CCPointMake(winSize.width / 2, quarterHeight / 2));
    
    _ball->setPosition(CCPointMake(winSize.width / 2, quarterHeight * 2));
    _ball->resetForces();
}


#pragma mark - b2ContactListener

void BYGameScene::BeginContact(b2Contact* contact) {
    CCLog("contact began");
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
    
    CCObject* objA = (CCObject*) bodyA->GetUserData();
    CCObject* objB = (CCObject*) bodyB->GetUserData();
    
    /// return if not ball
    if (objA != _ball
        || objB != _ball) {
        return ;
    }
    CCLog("ball contacted");
}


void BYGameScene::EndContact(b2Contact* contact) {
    
}


void BYGameScene::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    
}

void BYGameScene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
    CCLog("PostSolve contact");
}