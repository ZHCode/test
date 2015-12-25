//
//  MyTileset.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/19.
//
//

#ifndef BackGroundSprite_h
#define BackGroundSprite_h
#include "cocos2d.h"

USING_NS_CC;
using namespace std;
class BackGroundSprite : public Node
{
public:
    BackGroundSprite();
    ~BackGroundSprite(); //无派生类是可不设置为虚函数
    
    Texture2D * _texture; //瓦片所用纹理
    Rect _rect;  //纹理中所处的位置
    Vec2 _anchorPoint;
    Vec2 _postion; 
    
};

#endif /* MyTileset_h */
