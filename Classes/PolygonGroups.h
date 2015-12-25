//
//  PolygonGroups.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#ifndef PolygonGroups_h
#define PolygonGroups_h
#include "cocos2d.h"
using namespace std ;
USING_NS_CC;
class PolygonGroups : public Node
{
public:
    string name;
    int x;
    int y;
    int width;
    int height;
    int type;
    vector<vector<Vec2>>  points;  //多边形的顶点信息
    PolygonGroups();
    ~PolygonGroups();
};

#endif /* PolygonGroups_h */
