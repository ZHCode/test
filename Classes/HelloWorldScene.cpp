#include "HelloWorldScene.h"
#include <Box2D/Box2D.h>
#include "StageInfo.h"
#include "StageNode.h"
USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    //auto test = TMXTiledMap::create("test001.tmx");
    StageInfo *test = StageInfo::create("test001.tmx");
    
    StageNode * node = StageNode::createWithStageInfo(test);
    float width = node->getMapWidth();
    float posX = (visibleSize.width - width)/2;
    node->setPositionX(posX);
    this->addChild(node);
   
    return true;
}

void HelloWorld::update(float dt)
{
    
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
