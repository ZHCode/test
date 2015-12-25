//
//  StageNode.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#ifndef StageNode_h
#define StageNode_h
#include "cocos2d.h"
#include "StageInfo.h"
using namespace std;
USING_NS_CC;
class StageNode : public Node
{
private:
    float SHOW_HEIGHT;
    float limitTopHeight;// 开始加载瓦片的最大高度
    float limitDownHeight;// 开始加载瓦片的最低高度
    Vector<Sprite*> layerSpriteArray; //存储所有层的精灵
    Vector<Sprite*> ItemsSpriteArray; //储存所有group层 item精灵
    float speed; //地图移动速度
    float MapWidth; //背景地图宽度 用于设置居中显示
public:
    virtual void update(float dt);
    static StageNode* createWithStageInfo(StageInfo * info);
    bool initWithStageInfo(StageInfo * info);
    void setSpeed(float num);
    float getMapWidth();
};
#endif /* StageNode_h */
