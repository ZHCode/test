//
//  polylineGroups.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#ifndef PolylineGroups_h
#define PolylineGroups_h
#include "cocos2d.h"
using namespace std;
USING_NS_CC;
class PolylineGroups : public Node
{
public:
    string name;
    int x;
    int y;
    int width;
    int height;
    int type;
    vector<vector<Vec2>>  points;  //折线的顶点信息
    PolylineGroups();
    ~PolylineGroups();
};

#endif /* polylineGroups_h */
