//
//  Arrow.hpp
//  CocosGame
//
//  Created by Ammar Hussain on 01/01/20.
//

#ifndef Arrow_hpp
#define Arrow_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class Arrow : public Node
{
public:
    
    void update(float dt) override;
    virtual bool init() override;
};

#endif /* Arrow_hpp */
