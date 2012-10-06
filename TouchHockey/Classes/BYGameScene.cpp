//
//  BYGameScene.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYGameScene.h"
#include "BYPaddle.h"

using namespace cocos2d;


CCScene* BYGameScene::scene() {
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new BYGameScene();
    scene->addChild(layer);
    layer->autorelease();
    
    return scene;
}


BYGameScene::BYGameScene() {
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
    _topPaddle->setCenterAtPosition(CCPointMake(winSize.width / 2, quarterHeight));
    _topPaddle->autorelease();
    this->addChild(_topPaddle->getSprite());
    
    _botPaddle = new BYPaddle();
    _botPaddle->init(CCString::createWithFormat("paddle_green.png"));
    _botPaddle->setCenterAtPosition(CCPointMake(winSize.width / 2, quarterHeight *3));
    _botPaddle->autorelease();
    this->addChild(_botPaddle->getSprite());
    
    /// add ball
    _ball = new BYBall();
    _ball->init(CCString::createWithFormat("ball_red.png"));
    _ball->setCenterAtPosition(CCPointMake(winSize.width / 2, quarterHeight * 2));
    _ball->autorelease();
    this->addChild(_ball->getSprite());
    
    
    /// add world
    b2Vec2 gravity = b2Vec2(0,0);
    _world = new b2World(gravity);
}

BYGameScene::~BYGameScene() {
}

//
//
//void BYGameScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
//}
//
//
//void BYGameScene::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
//    /*
//     For single touch event process only single touch for each paddle.
//     Any paddle can receive only 1 touch
//     */
//    
//    bool didPassToTopPaddle = false;
//    bool didPassToBotPaddle = false;
//    static float halfY = CCDirector::sharedDirector()->getWinSize().height / 2;
//    
//    CCSetIterator iterator;
//    for(iterator = pTouches->begin();
//        iterator != pTouches->end();
//        iterator++)
//    {
//        CCTouch* touch = (CCTouch*)(*iterator);
//        CCPoint touchPoint = touch->getLocation();
//        
//        if (touchPoint.y > halfY) {
//            if (! didPassToTopPaddle) {
//                didPassToTopPaddle = true;
//                
//            }
//        }
//        
//        else {
//            if (! didPassToBotPaddle) {
//                didPassToBotPaddle = true;
//            }
//        }
//    }
//}
//
//void BYGameScene::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent) {
//    
//}