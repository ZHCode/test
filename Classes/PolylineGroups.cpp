//
//  polylineGroups.cpp
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#include "PolylineGroups.h"
PolylineGroups::PolylineGroups()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
    type = 0;
    name = "Polyline";
}
PolylineGroups::~PolylineGroups()
{
    CCLOG("PolylineGroups  release");
}