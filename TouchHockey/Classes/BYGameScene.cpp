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
    
    CCRect playArea = CCRectMake(contPoint.x,
                                 contPoint.y,
                                 winSize.width  - 2 * contPoint.x,
                                 winSize.height - 2 * contPoint.y);
    CCRect botRect(playArea);
    botRect.size.height /= 2;
    _botPaddle->setMoveArea(botRect);
    
    CCRect topRect = CCRectMake(contPoint.x,
                                contPoint.y + playArea.size.height / 2,
                                playArea.size.width,
                                playArea.size.height / 2);
    _topPaddle->setMoveArea(topRect);
    
    this->scheduleUpdate();
}


void BYGameScene::loadBoxWorld() {
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
}



BYGameScene::~BYGameScene() {
    delete _world;
    _topPaddle->release();
    _botPaddle->release();
    _ball->release();
}



#pragma mark - touch handle

void BYGameScene::ccTouchesBegan(cocos2d::CCSet *pTouches,
                                 cocos2d::CCEvent *pEvent) {
    
#warning TODO: check pause button
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
    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            b2Vec2 position = b->GetPosition();
            myActor->setPosition( CCPointMake( position.x * PTM_RATIO,
                                              position.y * PTM_RATIO));
        }
    }
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