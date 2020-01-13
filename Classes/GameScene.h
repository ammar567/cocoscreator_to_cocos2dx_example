//
//  GameScene.hpp
//  CocosGame
//
//  Created by Ammar Hussain on 10/01/20.
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "Definition.h"
#include "reader/CreatorReader.h"
#include "Arrow.h"

USING_NS_CC;

class GameScene : public Scene

{
public:
     Scene* createScene();
    
     creator::CreatorReader* reader;
    virtual  void update(float dt) override;
     virtual bool init();
     void init(Scene* scene);
    CREATE_FUNC(GameScene);
   
private:
    
};

#endif /* GameScene_hpp */
