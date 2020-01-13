//
//  GameMain.cpp
//  CocosGame
//
//  Created by Ammar Hussain on 09/12/19.
//

#include "GameMain.h"
#include "extensions/cocos-ext.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "B2DebugDrawLayer.h"
#include "GLES-Render.h"
#include "base/ccConfig.h"
#include "HelloWorldScene.h"




USING_NS_CC;

Scene* GameMain::createScene()
{
    
    return GameMain::create();
}

bool GameMain::init()
{
    if (!Scene::init()) {
        return false;
    }
    
    dragOffsetStartX = 0;
    dragOffsetEndX = 0;
    dragOffsetStartY = 0;
    dragOffsetEndY = 0;
    existBall= false;
    
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();

        b2Vec2 gravity = b2Vec2(0.0f,-10.0f);
        world = new b2World(gravity);
        world->SetAllowSleeping(true);
        world->SetContinuousPhysics(true);

        AddWall();
       
    for (int i = 0 ; i <= arrayLimit; i++){
        points[i] =Sprite::create("assets/dot.png");
        points[i]->setScale(0.25);
        this->addChild(points[i]);
    }
    
   
    indicator = Sprite::create("assets/arrw.png");
    //indicator->setScale(0.0625);
    //indicator->setAnchorPoint(Vec2(0,0));
    indicator->setPosition(Vec2(origin.x ,origin.y ));
    
   // node = Node::create();
    point = Sprite::create("assets/star.png");
    point->setScale(.25);
    
    //point->setVisible(false);
    
    
    
    
    log("indicator width %f",point->getContentSize().width * indicator->getScale());
   // point->setScale(0.5);
    point->setPosition(Vec2( indicator->getContentSize().width-10 ,indicator->getContentSize().height/2 ));
    log("point position %f",point->getPosition().x);
   
    indicator->addChild(point, 0, 2);
    this->addChild(indicator);
    log("point position world %f",WORLD_TO_SCREEN(250));
    Rect aabb = point->getBoundingBox();
    DrawNode* drawNode = DrawNode::create();
    drawNode->drawRect(aabb.origin, aabb.origin + aabb.size, Color4F(1, 0, 0, 1));
    auto touchlistener = EventListenerTouchOneByOne::create();
    touchlistener->onTouchBegan = CC_CALLBACK_2(GameMain::onTouchBegan,this);
    touchlistener->onTouchEnded = CC_CALLBACK_2(GameMain::onTouchEnded,this);
    touchlistener->onTouchMoved = CC_CALLBACK_2(GameMain::onTouchMoved,this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);
    
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameMain::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameMain::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);



  
          sprite = Sprite::create("assets/ball.png");
      
          // position the sprite on the center of the screen
       sprite->setScale(0.5);
       sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    
    
    
    b2BodyDef bodyDef;
    b2PolygonShape bodyShape;
    
    
    bodyShape.SetAsBox(8/PTM_RATIO, 8/PTM_RATIO);
    
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(250/PTM_RATIO, 200/PTM_RATIO);
    
    tempBody = world->CreateBody(&bodyDef);
    
           b2FixtureDef  fixtureDef;
           fixtureDef.density=1;
           fixtureDef.friction=0.8;
           fixtureDef.restitution=0.6;
           fixtureDef.shape=&bodyShape;
           tempBody->CreateFixture(&fixtureDef);

    
      // defineBall();
   
       auto debugDraw = B2DebugDrawLayer::create(world, PTM_RATIO);
       
      // this->addChild(sprite,-1);
       this->addChild(debugDraw, 10);
       scheduleUpdate();
    
  //  Helper::createRectangleBody(2, 2, 200, 200, true, world);
 //   helper.createRectangleBody(20, 20, 200, 200, true, world);
    return true;
    
    
    
    
}
void GameMain::defineBall()
{
//     b2BodyDef crateBodyDef;
//
//
//
//
//       b2PolygonShape crateShape;
//
//       crateShape.SetAsBox(((sprite->getContentSize().width/2 + 0.005)*sprite->getScale())/PTM_RATIO, ((sprite->getContentSize().height/2 +0.005)*sprite->getScale())/PTM_RATIO);
//
//       crateBodyDef.position.Set(sprite->getPosition().x / PTM_RATIO,sprite->getPosition().y/PTM_RATIO);
//       crateBodyDef.type = b2_dynamicBody;
//       crateBodyDef.userData = sprite;
//
//        crateBody = world->CreateBody(&crateBodyDef);
//
//       b2FixtureDef  fixtureDef;
//       fixtureDef.density=1;
//       fixtureDef.friction=0.8;
//       fixtureDef.restitution=0.6;
//       fixtureDef.shape=&crateShape;
//       crateBody->CreateFixture(&fixtureDef);
    
    
    ballShape.m_radius = (45 / PTM_RATIO)*sprite->getScale();
    b2FixtureDef ballFixture;
    ballFixture.density=10;
    ballFixture.friction=0.8;
    ballFixture.restitution=0.6;
    ballFixture.shape=&ballShape;

    ballBodyDef.type= b2_dynamicBody;
    ballBodyDef.userData=sprite;

    ballBodyDef.position.Set(sprite->getPosition().x/PTM_RATIO,sprite->getPosition().y/PTM_RATIO);

    body = world->CreateBody(&ballBodyDef);
    body->CreateFixture(&ballFixture);
    //body->SetGravityScale(10);
    
}

void GameMain::AddWall()
{
    // Define the ground body.
       b2BodyDef groundBodyDef;
       groundBodyDef.position.Set(0, 0); // bottom-left corner

       // Call the body factory which allocates memory for the ground body
       // from a pool and creates the ground box shape (also from a pool).
       // The body is also added to the world.
       b2Body* groundBody = world->CreateBody(&groundBodyDef);

       // Define the ground box shape.
       b2EdgeShape groundBox;

       // bottom
       groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
       groundBody->CreateFixture(&groundBox,0);

       // top
       groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
       groundBody->CreateFixture(&groundBox,0);

       // left
       groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
       groundBody->CreateFixture(&groundBox,0);

       // right
       groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
       groundBody->CreateFixture(&groundBox,0);
    
}
void GameMain::update(float delta)
{
     int positionIterations = 10;  // Location
       int velocityIterations = 10; // Velocity
    deltaTime = delta;
       world->Step(delta, velocityIterations, positionIterations);
        for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
       // Consider which body is attached to Sprite
            if (body->GetUserData())
            {

              Sprite *sprite = (Sprite *) body->GetUserData();
       // Set the position again for this Sprite follow the position of body ( body is gradually falling by time), remember to multiply with RATIO to convert into pixel
              sprite->setPosition(Point(body->GetPosition().x * PTM_RATIO,body->GetPosition().y * PTM_RATIO));
             // Set the rotation ability
              sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

            }
    world->ClearForces();
    
}

void GameMain::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);

    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    _modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(GameMain::onDraw, this);
    renderer->addCommand(&_customCommand);

    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);}

void GameMain::onDraw()
{
        Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");

    auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
    world->DrawDebugData();
    

    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}

bool GameMain::onTouchBegan(Touch *touch, Event *event)
{
//    auto scene = HelloWorld::createScene(this);
//
//    Director::getInstance()->pushScene(scene);
    
    // run
//    float mouseX = touch->getLocation().x;
//    float mouseY = touch->getLocation().y;
//    Vec2 mouseVec = Vec2(mouseX/PTM_RATIO,mouseY/PTM_RATIO);
//    log("Hello touch began x %f", mouseVec.x);
//    log("Hello touch began y %f", mouseVec.y);
//    Vec2 finalVec = Vec2(mouseVec.x,mouseVec.y);
//    finalVec.subtract(Vec2(crateBody->GetPosition().x,crateBody->GetPosition().y));
//    finalVec.normalize();
//    log("Hello touch began x %f", finalVec.x);
//    log("Hello touch began y %f", finalVec.y);
//    crateBody->SetLinearVelocity(b2Vec2(finalVec.x*8,finalVec.y*8));

//    createRectBody(0, 0, 0, 0);
    
    
       dragOffsetStartX = touch->getLocation().x;
       dragOffsetStartY = touch->getLocation().y;
       CCPoint touchLocation = touch->getLocation();
//
//       ballX = touchLocation.x;
//       ballY = touchLocation.y;
//
//       if (existBall){
//           world->DestroyBody(body);
//       }
//
//       sprite->setPosition(Vec2(ballX ,ballY));
    
    
    
    
    
//    log(" maximum height %f",getMaxHeight(b2Vec2(touch->getLocation().x/PTM_RATIO,touch->getLocation().y/PTM_RATIO),b2Vec2(5,8)));
//
//   // indicator->setPosition(Vec2(5*PTM_RATIO,getMaxHeight(b2Vec2(touch->getLocation().x/PTM_RATIO,touch->getLocation().y/PTM_RATIO),b2Vec2(5,8))*PTM_RATIO));
//    log(" vertical velocity %f",calculateVerticalVelocityForHeight(8));
   
    indicator->setPosition(Vec2(touch->getLocation().x,touch->getLocation().y));
    indicator->setRotation(0);
    
    log("point position %f",point->convertToWorldSpace(point->getPosition()).x);
    log("indicator position %f",indicator->getPosition().x);
    
   // point->setPosition(Vec2(indicator->getPosition().x + (indicator->getContentSize().width * indicator->getScale())/2,indicator->getPosition().y ));
    
    
    //log(" position x y %f",touch->getLocation().x + indicator->getContentSize().width/2, touch->getLocation().y);
  
    return true;
}
float GameMain::calculateVerticalVelocityForHeight( float desiredHeight )
{
    if ( desiredHeight <= 0 )
        return 0; //wanna go down? just let it drop

    //gravity is given per second but we want time step values here
    float t = 1 / 60.0f;
    b2Vec2 stepGravity = t * t * world->GetGravity(); // m/s/s

    //quadratic equation setup (ax² + bx + c = 0)
    float a = 0.5f / stepGravity.y;
    float b = 0.5f;
    float c = desiredHeight;
    
    //check both possible solutions
    float quadraticSolution1 = ( -b - b2Sqrt( b*b - 4*a*c ) ) / (2*a);
    float quadraticSolution2 = ( -b + b2Sqrt( b*b - 4*a*c ) ) / (2*a);

    //use the one which is positive
    float v = quadraticSolution1;
    if ( v < 0 )
        v = quadraticSolution2;

    //convert answer back to seconds
    return v * 60.0f;
}
float GameMain::getMaxHeight(b2Vec2 startingPosition, b2Vec2 startingVelocity)
{
    
    //if the projectile is already heading down, this is as high as it will get
        if ( startingVelocity.y < 0 )
            return startingPosition.y;
    
        //velocity and gravity are given per second but we want time step values here
        float t = 1 / 60.0f;
        b2Vec2 stepVelocity = t * startingVelocity; // m/s
        b2Vec2 stepGravity = t * t * world->GetGravity(); // m/s/s
    
        //find n when velocity is zero
        float n = -stepVelocity.y / stepGravity.y - 1;
    
        //plug n into position formula, using only vertical components
        return startingPosition.y + n * stepVelocity.y + 0.5f * (n*n+n) * stepGravity.y;
}



void GameMain::onTouchMoved(Touch *touch, Event *event)
{
    
    
    CCPoint touchLocation = touch->getLocation();

    dragOffsetEndX = touchLocation.x;
    dragOffsetEndY = touchLocation.y;

    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

   // GameMain::simulateTrajectory(b2Vec2((dragDistanceX * 10)/PTM_RATIO,(dragDistanceY * 10)/PTM_RATIO));
    rotateNodeToPoint(indicator, Point(touch->getLocation().x,touch->getLocation().y));
    for (int i = 0; i < arrayLimit; i++) { // three seconds at 60fps
        Vec2 pos = point->convertToWorldSpace(point->getPosition());
       // pos.x = pos.x - 5;
         b2Vec2 trajectoryPosition = getTrajectoryPoint( b2Vec2((pos.x )/PTM_RATIO,pos.y/PTM_RATIO),b2Vec2((dragDistanceX*50)/PTM_RATIO,(dragDistanceY*50)/PTM_RATIO),i);
        points[i]->setScale(0.25);
        points[i]->setPosition(Vec2(trajectoryPosition.x*PTM_RATIO ,trajectoryPosition.y*PTM_RATIO));
    }
       touchX = touch->getLocation().x;
       touchY= touch->getLocation().y;
       velocityX = (dragDistanceX*50)/PTM_RATIO;
       velocityY = (dragDistanceY*50)/PTM_RATIO;
    
       
    
    log("child %f",point->convertToWorldSpace(point->getPosition()).x);
    
    Vec2 indiVec = Vec2(indicator->getPosition().x,indicator->getPosition().y);
    
    
}

void GameMain::onTouchEnded(Touch *touch, Event *event)
{
    tempBody->SetTransform(b2Vec2((point->convertToWorldSpace(point->getPosition()).x )/PTM_RATIO,( point->convertToWorldSpace(point->getPosition()).y  )/PTM_RATIO), tempBody->GetAngle());
    tempBody->SetLinearVelocity(b2Vec2(velocityX,velocityY));
//    existBall = true;
//
//    defineBall();
//
//    CCPoint touchLocation = touch->getLocation();
//
//    dragOffsetEndX = touchLocation.x;
//    dragOffsetEndY = touchLocation.y;
//
//    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
//    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;
//
//    body->SetLinearVelocity(b2Vec2((dragDistanceX * 10)/PTM_RATIO,(dragDistanceY * 10)/PTM_RATIO));
     //tempBody->SetLinearVelocity(b2Vec2(50,8));
}

void GameMain::simulateTrajectory(b2Vec2 coord)
{
    GameMain::defineBall();

    body->SetLinearVelocity(b2Vec2(coord.x,coord.y));
    for (int i = 1; i <= 14; i++){
        world->Step(deltaTime,10,10);
        points[i]->setScale(0.25);
    points[i]->setPosition(CCPoint(body->GetPosition().x*PTM_RATIO,body->GetPosition().y*PTM_RATIO));
    world->ClearForces();
        
    }

    world->DestroyBody(body);
}

b2Vec2 GameMain::getTrajectoryPoint(b2Vec2 startingPosition, b2Vec2 startingVelocity, float n)
{
    
    //velocity and gravity are given per second but we want time step values here
        float t = 1 / 60.0f; // seconds per time step (at 60fps)
        b2Vec2 stepVelocity = t * startingVelocity; // m/s
        b2Vec2 stepGravity = t * t * world->GetGravity(); // m/s/s
    
        return startingPosition + n * stepVelocity + 0.5f * (n*n+n) * stepGravity;
}


void GameMain::Shoot()
{
    
}

void GameMain::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
    tempBody->SetTransform(b2Vec2((point->convertToWorldSpace(point->getPosition()).x )/PTM_RATIO,( point->convertToWorldSpace(point->getPosition()).y  )/PTM_RATIO), tempBody->GetAngle());
    tempBody->SetLinearVelocity(b2Vec2(velocityX,velocityY));
    
}

void GameMain::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    
}

float GameMain::getCurrentAngle(cocos2d::Node *node)
{
    float rotAng = node->getRotation();

    if(rotAng >= 180.f)
    {
        rotAng -= 360.f;
    }
    else if (rotAng < -180.f)
    {
        rotAng +=360.f;
    }

    // negative angle means node is facing to its left
    // positive angle means node is facing to its right
    return rotAng;
}


float GameMain::getAngleDifference(float angle1, float angle2)
{
    float diffAngle = (angle1 - angle2);

    if(diffAngle >= 180.f)
    {
        diffAngle = 360.f;
    }
    else if (diffAngle < -180.f)
    {
        diffAngle +=360.f;
    }

    // how much angle the node needs to rotate
    return diffAngle;
}

float GameMain::getAngleOfTwoVectors(cocos2d::Point vec1, cocos2d::Point vec2)
{
    auto vectorFromVec1ToVec2 = vec1 - vec2;
    // the angle between two vectors
    return CC_RADIANS_TO_DEGREES(-vectorFromVec1ToVec2.getAngle());
}


void GameMain::rotateNodeToPoint(cocos2d::Node *node, cocos2d::Point point)
{
    float angleNodeToRotateTo = getAngleOfTwoVectors(indicator->getPosition(), point);
    float nodeCurrentAngle = getCurrentAngle(indicator);
    float diffAngle = getAngleDifference(angleNodeToRotateTo, nodeCurrentAngle);
    float rotation = nodeCurrentAngle + diffAngle;
    indicator->setRotation(rotation);
}


void GameMain::createRectBody(float width, float height, float posX, float posY,bool isStatic,Sprite* sprite)
{
    b2BodyDef bdyDef;
    b2PolygonShape bdyShape;
    b2Body*   bdy;
    
    bdyShape.SetAsBox(width/PTM_RATIO, height/PTM_RATIO);
    
    if (isStatic) {
        bdyDef.type = b2_staticBody;
    }
    else{
        bdyDef.type = b2_dynamicBody;
        bdyDef.userData = sprite;
    }
    bdyDef.position.Set(posX/PTM_RATIO, posY/PTM_RATIO);
    
      bdy = world->CreateBody(&bdyDef);
    
           b2FixtureDef  fixDef;
           fixDef.density=1;
           fixDef.friction=0.8;
           fixDef.restitution=0.6;
           fixDef.shape=&bdyShape;
           bdy->CreateFixture(&fixDef);
}

void GameMain::debug()
{
    log("GameMain debug");
}
