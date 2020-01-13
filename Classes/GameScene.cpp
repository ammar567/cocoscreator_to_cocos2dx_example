
#include "GameScene.h"

Scene* GameScene::createScene()
{
      reader = creator::CreatorReader::createWithFilename("creator/Scene/helloworld.ccreator");
      reader->setup();
      Scene * GameScene = reader->getSceneGraph();
      GameScene::init(GameScene);
      
    return GameScene;
}

void GameScene::init(Scene *scene)
{
    auto node =   scene->getChildByName("Canvas");
    scene->schedule(CC_SCHEDULE_SELECTOR(GameScene::update), 0.5f);
    scene->schedule(CC_SCHEDULE_SELECTOR(GameScene::update), 0.5f);
}


bool GameScene::init()
{
    return true;

    
}

void GameScene::update(float dt)
{
    log("scene start");
    
}
