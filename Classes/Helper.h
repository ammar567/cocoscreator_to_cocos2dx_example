//
//  Helper.hpp
//  CocosGame
//
//  Created by Ammar Hussain on 10/01/20.
//

#ifndef Helper_hpp
#define Helper_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Definition.h"

USING_NS_CC;

class Helper
{
public:
    
    void createRectangleBody(float width,float height,float posX,float posY,bool isStatic,b2World* world,Sprite* sprite);
     
    
};
#endif /* Helper_hpp */
