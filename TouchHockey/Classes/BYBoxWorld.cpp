//
//  BYBoxWorld.cpp
//  TouchHockey
//
//  Created by myeyesareblind on 10/6/12.
//
//

#include "BYBoxWorld.h"


BYBoxWorld::BYBoxWorld() {
    /// add world
    b2Vec2 gravity = b2Vec2(0,0);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(false);
    _world->SetContinuousPhysics(true);
}

BYBoxWorld::~BYBoxWorld() {
    delete _world;
}

b2World* BYBoxWorld::getWorld() {
    return _world;
}