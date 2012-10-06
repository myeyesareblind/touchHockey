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
#include "BYBoxWorld.h"

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
    this->addChild(background);
    
    /// add players
    float quarterHeight = (float) winSize.height / 4;
    _topPaddle = new BYPaddle();
    _topPaddle->init(CCString::createWithFormat("paddle_blue.png"));
    _topPaddle->setCenterAtPosition(CCPointMake(winSize.width / 2,  quarterHeight *3));
    this->addChild(_topPaddle->getSprite());
    
    _botPaddle = new BYPaddle();
    _botPaddle->init(CCString::createWithFormat("paddle_green.png"));
    _botPaddle->setCenterAtPosition(CCPointMake(winSize.width / 2, quarterHeight));
    this->addChild(_botPaddle->getSprite());

    
    /// add ball
    _ball = new BYBall();
    _ball->init(CCString::createWithFormat("ball_red.png"));
    _ball->setCenterAtPosition(CCPointMake(winSize.width / 2, quarterHeight * 2));
    this->addChild(_ball->getSprite());
    
    CCPoint contPoint = boardCornerPoint();
    
    _playArea = CCRectMake(contPoint.x,
                           contPoint.y,
                           winSize.width  - 2 * contPoint.x,
                           winSize.height - 2 * contPoint.y);
    
    _paddleBotArea = CCRect(_playArea);
    _paddleBotArea.size.height /= 2;
    
    _paddleTopArea = CCRect(_playArea);
    _paddleTopArea.size.height /= 2;
    _paddleTopArea.origin.y = winSize.height / 2;
    
    this->loadBoxWorld();
    this->scheduleUpdate();
}


void BYGameScene::loadBoxWorld() {
    BYBoxWorld& byWorld = BYBoxWorld::BoxWorldInstance();
    _world = byWorld.getWorld();
    
    /// add bounds
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    b2Body *groundBody = _world->CreateBody(& groundBodyDef);
    b2EdgeShape groundBox;
    
    
    CCPoint contPoint = this->boardCornerPoint();
    float   boardShortStickLenght = boardShortStickLength();
    
    /// bottom left
    CCPoint botLeftPadPoint = CCPoint(contPoint);
    botLeftPadPoint.x += boardShortStickLenght;
    groundBox.Set(vecFromPoint(contPoint),
                  vecFromPoint(botLeftPadPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// left
    CCPoint topLeftPoint = CCPoint(contPoint);
    topLeftPoint.y = s.height - 2 * contPoint.y;
    groundBox.Set(vecFromPoint(contPoint),
                  vecFromPoint(topLeftPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// top left
    CCPoint topLeftPadPoint = CCPoint(topLeftPoint);
    topLeftPadPoint.x += boardShortStickLenght;
    groundBox.Set(vecFromPoint(topLeftPoint),
                  vecFromPoint(topLeftPadPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    
    /// bot right
    CCPoint botRightPoint = CCPointMake(s.width - 2 * contPoint.x, contPoint.y);
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(CCPointMake(botRightPoint.x - boardShortStickLenght,
                                           botLeftPadPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// right
    CCPoint topRightPoint = CCPoint(botRightPoint);
    topRightPoint.y = s.height - 2 * botLeftPadPoint.y;
    groundBox.Set(vecFromPoint(botRightPoint),
                  vecFromPoint(topRightPoint));
    groundBody->CreateFixture(&groundBox, 0);
    
    /// topRight
    groundBox.Set(vecFromPoint(topRightPoint),
                  vecFromPoint(CCPointMake(topRightPoint.x - boardShortStickLenght, topRightPoint.y)));
    groundBody->CreateFixture(&groundBox, 0);    
}


BYGameScene::~BYGameScene() {
    delete _world;
    _topPaddle->release();
    _botPaddle->release();
    _ball->release();
}

//
//
void BYGameScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
}


void BYGameScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    /*
     For single touch event process only single touch for each paddle.
     Any paddle can receive only 1 touch
     */
    
    bool didPassToTopPaddle = false;
    bool didPassToBotPaddle = false;
    static float halfY = CCDirector::sharedDirector()->getWinSize().height / 2;
    
    CCSetIterator iterator;
    for(iterator = pTouches->begin();
        iterator != pTouches->end();
        iterator++)
    {
        CCTouch* touch = (CCTouch*)(*iterator);
        CCPoint touchPoint = touch->getLocation();
        
        /// is point inside playRect
        if (! _playArea.containsPoint(touchPoint)) {
            continue ;
        }
        
        CCPoint previousPoint = touch->getPreviousLocation();
        
        if (touchPoint.y > halfY) {
            if (! didPassToTopPaddle) {
                if (_topPaddle->containsPoint(previousPoint)) {
                    _topPaddle->setCenterAtPosition(touchPoint);
                    didPassToTopPaddle = true;
                }
            }
        }
        
        else {
            if (! didPassToBotPaddle) {
                if (_botPaddle->containsPoint(previousPoint)) {
                    _botPaddle->setCenterAtPosition(touchPoint);
                    didPassToBotPaddle = true;
                }
            }
            
        } /// else
    } /// iterator
}


void BYGameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
    
}


void BYGameScene::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
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
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO,
                                              b->GetPosition().y * PTM_RATIO) );
            
        }
    }
}


//#ifdef DEBUG
//void BYGameScene::draw()
//{
//	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
//	// Needed states:  GL_VERTEX_ARRAY,
//	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
//	glDisable(GL_TEXTURE_2D);
//	glDisableClientState(GL_COLOR_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	
//	world->DrawDebugData();
//	
//	// restore default GL states
//	glEnable(GL_TEXTURE_2D);
//	glEnableClientState(GL_COLOR_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//}
//#endif
