//
//  MapGroups.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/22.
//
//

#ifndef ItemGroups_h
#define ItemGroups_h
#include "cocos2d.h"
USING_NS_CC;
using namespace std;
class ItemGroups : public Node
{
public:
    string name;
    int type ; // 类型用于后续通过类型来 添加不同的属性
    int width; // 逻辑判断区的宽
    int height;// 逻辑判断区的高
    int x ;    // 坐标x
    int y ;    // y 坐标
    Rect rect; // 纹理rect
    
    ItemGroups();
    ~ItemGroups();
};

#endif /* MapGroups_h */
