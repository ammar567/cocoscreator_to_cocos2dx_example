//
//  Helper.cpp
//  CocosGame
//
//  Created by Ammar Hussain on 10/01/20.
//

#include "Helper.h"

void Helper::createRectangleBody(float width, float height, float posX, float posY, bool isStatic,b2World* world,Sprite* sprite)
{
    
       b2BodyDef bodyDef;
       b2PolygonShape bodyShape;
       b2Body* body;
       
       
       bodyShape.SetAsBox(SCREEN_TO_WORLD(width), SCREEN_TO_WORLD(height));
       
    if (isStatic) {
        bodyDef.type = b2_staticBody;
    }
    else{
        bodyDef.type = b2_dynamicBody;
    }
       
       bodyDef.position.Set(SCREEN_TO_WORLD(posX), SCREEN_TO_WORLD(posY));
       
       body = world->CreateBody(&bodyDef);
       
              b2FixtureDef  fixtureDef;
              fixtureDef.density=1;
              fixtureDef.friction=0.8;
              fixtureDef.restitution=0.6;
              fixtureDef.shape=&bodyShape;
              body->CreateFixture(&fixtureDef);
}
