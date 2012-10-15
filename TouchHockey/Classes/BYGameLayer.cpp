//
//  BYGameLayer.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/14/12.
//
//

#include "BYGameLayer.h"
#include "BYGameScene.h"
#include "ConstantsAndMacros.h"

using namespace cocos2d;


const float     GOAL_SIZE   =   0.45f;


BYGameLayer::BYGameLayer() {
    
    CCPoint cornerPoint = m_GameLogic->boardCornerPoint();
    this->setPosition(cornerPoint);
    
    CCSize winSize = CCDirectorManager->getWinSize();
    this->setContentSize(CCSizeMake(winSize.width - 2 * cornerPoint.x,
                                    winSize.height - 2 * cornerPoint.y));
    
    this->setTouchEnabled(true);
        
    this->loadBoxWorld();
    
    this->scheduleUpdate();
}



BYGameLayer::~BYGameLayer() {
    
    delete m_world;
    m_topPaddle->release();
    m_botPaddle->release();
    m_ball->release();
}



bool BYGameLayer::initWithMultiPlayer() {
    /// add players
    CCSize winSize = this->getContentSize();
    
    float quarterHeight = (float) winSize.height / 4;
    m_topPaddle = new BYPlayerPaddle();
    m_topPaddle->init(CCString::createWithFormat("paddle_yellow.png"),
                     m_world,
                     CCPointMake(winSize.width / 2,  quarterHeight *3));
    this->addChild(m_topPaddle->getSprite());
    
    m_botPaddle = new BYPlayerPaddle();
    m_botPaddle->init(CCString::createWithFormat("paddle_green.png"),
                     m_world,
                     CCPointMake(winSize.width / 2, quarterHeight / 2));
    this->addChild(m_botPaddle->getSprite());
    
    
    /// add ball
    m_ball = new BYBall();
    m_ball->init(CCString::createWithFormat("ball_blue.png"),
                m_world,
                CCPointMake(winSize.width / 2, quarterHeight * 2));
    this->addChild(m_ball->getSprite());
    
    m_GameMode  = GameMode_Multiplayer;
    
    m_aiPaddle  = NULL;
    
    return true;
}



bool BYGameLayer::initWithSinglePlayer() {
    /// add players
    CCSize winSize = this->getContentSize();
    
    float quarterHeight = (float) winSize.height / 4;
    
    m_topPaddle = NULL;
    
    m_aiPaddle  = new BYAIPaddle();
    m_aiPaddle->init(CCString::createWithFormat("paddle_yellow.png"),
                     m_world,
                     CCPointMake(winSize.width / 2,  quarterHeight *3));
    this->addChild(m_aiPaddle->getSprite());
    
    
    m_botPaddle = new BYPlayerPaddle();
    m_botPaddle->init(CCString::createWithFormat("paddle_green.png"),
                      m_world,
                      CCPointMake(winSize.width / 2, quarterHeight / 2));
    this->addChild(m_botPaddle->getSprite());
    
    
    /// add ball
    m_ball = new BYBall();
    m_ball->init(CCString::createWithFormat("ball_blue.png"),
                 m_world,
                 CCPointMake(winSize.width / 2, quarterHeight * 2));
    this->addChild(m_ball->getSprite());
    
    m_GameMode  = GameMode_SinglePlayer;
    
    return true;
}


#pragma mark - Box2D handle


void BYGameLayer::loadBoxWorld() {
    CCAssert(! CCSizeZero.equals(this->getContentSize()), "Size should be setted!");
    
    b2Vec2 gravity = b2Vec2(0, 0);
    m_world = new b2World(gravity);
    m_world->SetAllowSleeping(false);
    m_world->SetContinuousPhysics(true);
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
    
    CCSize s = this->getContentSize();
    float   boardShortStickLenght = (s.width - s.width * GOAL_SIZE) / 2;
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    b2Body *groundBody = m_world->CreateBody(& groundBodyDef);
    b2EdgeShape groundBox;
    
    
    /// bottom left
    CCPoint botLeftPoint = CCPoint(0, 0);
    CCPoint botLeftPadPoint = CCPoint(botLeftPoint);
    botLeftPadPoint.x += boardShortStickLenght;
    groundBox.Set(vecFromPoint(botLeftPoint),
                  vecFromPoint(botLeftPadPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// left
    CCPoint topLeftPoint = CCPoint(botLeftPoint);
    topLeftPoint.y = s.height;
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
    CCPoint botRightPoint = CCPointMake(s.width, botLeftPoint.y);
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(CCPointMake(botRightPoint.x - boardShortStickLenght,
                                           botLeftPadPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// right
    CCPoint topRightPoint = CCPoint(botRightPoint);
    topRightPoint.y = s.height;
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(topRightPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// topRight
    groundBox.Set(vecFromPoint(topRightPoint),
                  vecFromPoint(CCPointMake(topRightPoint.x - boardShortStickLenght, topRightPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);
}


#pragma mark - touch handle

void BYGameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches,
                                 cocos2d::CCEvent *pEvent) {
    
    this->delegateTouchesToPaddles(pTouches);
}


void BYGameLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    
    this->delegateTouchesToPaddles(pTouches);
}


void BYGameLayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    /// empty
}


void BYGameLayer::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    /// empty
}


void BYGameLayer::delegateTouchesToPaddles(CCSet* pTouches) {
    /*
     For single touch event process only single touch for each paddle.
     Any paddle can receive only 1 touch
     */
    
    bool passedToTopPaddle = false;
    bool passedToBotPaddle = false;
    static float halfY = this->getContentSize().height / 2;
    
    CCSetIterator iterator;
    for(iterator = pTouches->begin();
        iterator != pTouches->end();
        iterator++)
    {
        CCTouch* touch = (CCTouch*)(*iterator);
        CCPoint touchPoint = touch->getLocation();
        touchPoint.x -= this->getPosition().x;
        touchPoint.y -= this->getPosition().y;
                
        if (touchPoint.y > halfY) {
            if (! passedToTopPaddle) {
                m_topPaddle->handleTouchAtPoint(touchPoint);
                passedToBotPaddle = true;
            }
        }
        
        else {
            if (! passedToBotPaddle) {
                m_botPaddle->handleTouchAtPoint(touchPoint);
                passedToBotPaddle = true;
            }
            
        } /// else
    } /// iterator
}


void BYGameLayer::update(float dt) {
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    CCSprite *ballSprite = m_ball->getSprite();
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
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


void BYGameLayer::resetGameObjects() {
    
    CCSize winSize = this->getContentSize();
    
    float quarterHeight = (float) winSize.height / 4;
    m_topPaddle->setPosition(CCPointMake(winSize.width / 2,  quarterHeight *3));
    m_botPaddle->setPosition(CCPointMake(winSize.width / 2, quarterHeight / 2));
    
    m_ball->setPosition(CCPointMake(winSize.width / 2, quarterHeight * 2));
    m_ball->resetForces();
}



void BYGameLayer::checkIfBallScored(const CCPoint& ballPoint) {
    static float height = this->getContentSize().height;
    
    if (ballPoint.y < 0 || ballPoint.y > height ) {
        
        if (m_GameLogic) {
            m_GameLogic->playerDidScore(ballPoint.y < 0 ?
                                        BYGamePlayer_topPlayer :
                                        BYGamePlayer_botPlayer);
        }
    }
}



void BYGameLayer::pauseWorld() {
    
    if (this->isTouchEnabled()) {
        this->setTouchEnabled(false);
        this->unscheduleUpdate();
    }
}



void BYGameLayer::resumeWorld() {
    
    if (! this->isTouchEnabled()) {
        this->setTouchEnabled(true);
        this->scheduleUpdate();
    }
}


