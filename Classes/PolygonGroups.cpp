//
//  PolygonGroups.cpp
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#include "PolygonGroups.h"
PolygonGroups::PolygonGroups()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    type = 0;
    name = "polygon";
}
PolygonGroups::~PolygonGroups()
{
    CCLOG("PolygonGroups  release");
}