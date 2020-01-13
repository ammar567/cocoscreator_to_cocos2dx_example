#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>

#include "GameMain.h"
#include "Helper.h"
#include "reader/CreatorReader.h"

USING_NS_CC;


class HelloWorld : public cocos2d::Scene, public b2ContactListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    //creator::CreatorReader reader;
    b2World *world;
    float deltaTime;
    Helper helper;
virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
 cocos2d::Mat4 _modelViewMV;
    Sprite *ball;
    bool existBall;
    float ballX;
    float ballY;
    int dragOffsetStartX;
    int dragOffsetEndX;
    int dragOffsetStartY;
    int dragOffsetEndY;
    float powerMultiplier;
    b2Body *ballBody;
    b2CircleShape ballShape;
    b2BodyDef ballBodyDef;
    void defineBall();
    GameMain *gameMain;
    
    Sprite *points[51];

    void addWall(float w,float h,float px,float py);
    void simulateTrajectory(b2Vec2 coord);

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onDraw();
    cocos2d::CustomCommand _customCommand;

    void update(float dt);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
