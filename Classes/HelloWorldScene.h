#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{
private:
    float SHOW_HEIGHT;
    float limitTopHeight;// 开始加载瓦片的最大高度
    float limitDownHeight;// 开始加载瓦片的最低高度
    Vector<Sprite*> layerSpriteArray; //存储所有层的精灵
    Vector<Sprite*> groupSpriteArray; //储存所有group层精灵
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual void update(float dt);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
