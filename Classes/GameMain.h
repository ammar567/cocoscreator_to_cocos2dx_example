//  GameMain.hpp
//  CocosGame
//  Created by Ammar Hussain on 09/12/19.

#ifndef GameMain_hpp
#define GameMain_hpp

#include <stdio.h>
#include <Box2D/Box2d.h>
#include "VisibleRect.h"
#include "Arrow.h"
#include "Definition.h"
#include "Helper.h"
USING_NS_CC;

class GameMain : public Scene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;    // implement the "static create()" method manually
    CREATE_FUNC(GameMain);
    void update(float dt);
    void AddWall();
    b2World *world;
    void defineBall();
    bool onTouchBegan(Touch *touch,Event * event);
    void onTouchEnded(Touch *touch,Event * event);
    void onTouchMoved(Touch *touch,Event * event);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void debug();
    Helper helper;
private:
    cocos2d::Mat4 _modelViewMV;
    bool existBall;
    Sprite *points[501];
    Sprite *indicator;
    float deltaTime=0;
    float ballX;
    float ballY;
    int dragOffsetStartX;
    int arrayLimit = 50;
    int dragOffsetEndX;
    int dragOffsetStartY;
    int dragOffsetEndY;
    
    Sprite *sprite;
    Sprite *point;
    Arrow *arrow;
    float maxHeight,velocityMax,touchX,touchY,velocityX,velocityY;
    void onDraw();
    cocos2d::CustomCommand _customCommand;
    b2World getPhysicsWorld();
     // World with physic
    b2Body *body;
    b2Body *tempBody;
    b2CircleShape ballShape;
    b2BodyDef ballBodyDef;
    Node *node;
    void simulateTrajectory(b2Vec2 coord);
    b2Vec2 getTrajectoryPoint(b2Vec2 startingPosition,b2Vec2 startingVelocity , float n);    
    float getMaxHeight(b2Vec2 startingPosition , b2Vec2 startingVelocity);
    float calculateVerticalVelocityForHeight(float desiredHeight);
    float getCurrentAngle(cocos2d::Node* node);
    float getAngleDifference(float angle1, float angle2);
    float getAngleOfTwoVectors(cocos2d::Point vec1, cocos2d::Point vec2);
    void rotateNodeToPoint(cocos2d::Node* node, cocos2d::Point point);
    void createRectBody(float width,float height,float posX,float posY,bool isStatic,Sprite* sprite);
    void Shoot();
};




#endif /* GameMain_hpp */
