#include "HelloWorldScene.h"
#include "B2DebugDrawLayer.h"
#include "GLES-Render.h"


USING_NS_CC;

#define SCALE_RATIO 48.0
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();

    auto layer = HelloWorld::create();

    scene->addChild(layer);

    
    
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //SET MOUSE LISTENER
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //END MOUSE LISTENER

    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);

    //CREATE A BALL
    dragOffsetStartX = 0;
    dragOffsetEndX = 0;
    dragOffsetStartY = 0;
    dragOffsetEndY = 0;
    existBall= false;
    ballX = 500;
    ballY = 200;
    powerMultiplier = 500;
    ball =Sprite::create("assets/ball.png");
    ball->setScale(0.5);
    ball->setPosition(CCPoint(ballX,ballY));
    this->addChild(ball);

    //HelloWorld::defineBall();
    
    helper.createRectangleBody(2, 60, 300, 100, true, world, ball);
    helper.createRectangleBody(2, 60, 100, 225, true, world, ball);


    addWall(visibleSize.width ,10,(visibleSize.width / 2) ,0); //CEIL
    addWall(10 ,visibleSize.height ,0,(visibleSize.height / 2) ); //LEFT
    addWall(10 ,visibleSize.height ,visibleSize.width,(visibleSize.height / 2) ); //RIGHT

    for (int i = 1 ; i <= 50; i++){
        points[i] =CCSprite::create("assets/dot.png");
        points[i]->setScale(0.5);
        this->addChild(points[i]);
    }
    auto debugDraw = B2DebugDrawLayer::create(world, PTM_RATIO);
          
         // this->addChild(sprite,-1);
          this->addChild(debugDraw, 10);
    scheduleUpdate();
    return true;
}

//Simulate Physics
void HelloWorld::update(float dt){
   int positionIterations = 10;
   int velocityIterations = 10;
    log("scene start");
   deltaTime = dt;
   world->Step(dt, velocityIterations, positionIterations);
  
   for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
     if (body->GetUserData())
     {
       CCSprite *sprite = (CCSprite *) body->GetUserData();
       sprite->setPosition(ccp(body->GetPosition().x * SCALE_RATIO,body->GetPosition().y * SCALE_RATIO));
       sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));



     }
    world->ClearForces();
    world->DrawDebugData();
}

void HelloWorld::addWall(float w,float h,float px,float py) {

    b2PolygonShape floorShape;

    floorShape.SetAsBox(w/ SCALE_RATIO,h/ SCALE_RATIO);
    b2FixtureDef floorFixture;
    floorFixture.density=0;
    floorFixture.friction=10;
    floorFixture.restitution=0.5;
    floorFixture.shape=&floorShape;
    b2BodyDef floorBodyDef;

    floorBodyDef.position.Set(px/ SCALE_RATIO,py/ SCALE_RATIO);
    b2Body *floorBody = world->CreateBody(&floorBodyDef);

    floorBody->CreateFixture(&floorFixture);

}

void HelloWorld::defineBall(){
    ballShape.m_radius = ((ball->getContentSize().width/2) / SCALE_RATIO)*ball->getScale();
    b2FixtureDef ballFixture;
    ballFixture.density=10;
    ballFixture.friction=0.8;
    ballFixture.restitution=0.6;
    ballFixture.shape=&ballShape;

    ballBodyDef.type= b2_dynamicBody;
    ballBodyDef.userData=ball;

    ballBodyDef.position.Set(ball->getPosition().x/SCALE_RATIO,ball->getPosition().y/SCALE_RATIO);

    ballBody = world->CreateBody(&ballBodyDef);
    ballBody->CreateFixture(&ballFixture);
    //ballBody->SetGravityScale(10);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
//    dragOffsetStartX = touch->getLocation().x;
//    dragOffsetStartY = touch->getLocation().y;
//
//
//    CCPoint touchLocation = touch->getLocation();
//
//    ballX = touchLocation.x;
//    ballY = touchLocation.y;
//
//    if (existBall){
//        world->DestroyBody(ballBody);
//    }
//
//    ball->setPosition(Vec2(ballX ,ballY));
    
    

    // will create the needed spritesheets + design resolution
   
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    CCPoint touchLocation = touch->getLocation();

    dragOffsetEndX = touchLocation.x;
    dragOffsetEndY = touchLocation.y;

    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

    //HelloWorld::simulateTrajectory(b2Vec2((dragDistanceX )/SCALE_RATIO,(dragDistanceY )/SCALE_RATIO));
    HelloWorld::simulateTrajectory(b2Vec2((dragDistanceX * powerMultiplier)/SCALE_RATIO,(dragDistanceY * powerMultiplier)/SCALE_RATIO));
 
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    existBall = true;

    HelloWorld::defineBall();

    CCPoint touchLocation = touch->getLocation();

    dragOffsetEndX = touchLocation.x;
    dragOffsetEndY = touchLocation.y;

    float dragDistanceX = dragOffsetStartX - dragOffsetEndX;
    float dragDistanceY = dragOffsetStartY - dragOffsetEndY;

    //ballBody->SetLinearVelocity(b2Vec2((dragDistanceX)/SCALE_RATIO,(dragDistanceY)/SCALE_RATIO));
    ballBody->SetLinearVelocity(b2Vec2((dragDistanceX * powerMultiplier)/SCALE_RATIO,(dragDistanceY * powerMultiplier)/SCALE_RATIO));
    
   
    
    
    
    creator::CreatorReader* reader = creator::CreatorReader::createWithFilename("creator/Scene/helloworld.ccreator");

    // will create the needed spritesheets + design resolution
    reader->setup();
    
    

    // get the scene graph
    Scene* scene = reader->getSceneGraph();
    log("size scene %f",scene->getContentSize().width);
    scene->scheduleUpdate();
    Director::getInstance()->replaceScene(scene);
}

void HelloWorld::simulateTrajectory(b2Vec2 coord){

    //define ball physicis
    HelloWorld::defineBall();

    ballBody->SetLinearVelocity(b2Vec2(coord.x,coord.y));
    for (int i = 1; i <= 50; i++){
        world->Step(deltaTime,10,10);
        points[i]->setPosition(CCPoint(ballBody->GetPosition().x*SCALE_RATIO,ballBody->GetPosition().y*SCALE_RATIO));
        world->ClearForces();
            
    }

    world->DestroyBody(ballBody);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);

    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    _modelViewMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    renderer->addCommand(&_customCommand);

    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);}

void HelloWorld::onDraw()
{
        Director* director = Director::getInstance();
    CCASSERT(nullptr != director, "Director is null when setting matrix stack");

    auto oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewMV);
    world->DrawDebugData();
    

    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
}
