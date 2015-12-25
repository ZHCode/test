//
//  ParseTmxMap.cpp
//  cocos2dXTest
//
//  Created by 1 on 15/12/18.
//
//

#include "StageInfo.h"//1001:polygon  1002:polyline  1003:items
enum  TYPE
{
    POLYGON = 1001,
    POLYLINE = 1002,
    ITEMS = 1003
    
};

StageInfo* StageInfo::create(const string &tmxFile)
{
    if(!tmxFile.length())
    {
        return nullptr;
    }
    StageInfo *ret = new  StageInfo();
    if (ret && ret->initTmxMapInfo(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    else
        return nullptr;
}
#pragma mark 解析获取地图信息
bool StageInfo::initTmxMapInfo(const string &tmxFile)
{
    isInit = false;
    
    this->mapSize.width = 0;
    this->mapSize.height = 0;
    mapInfo = TMXMapInfo::create(tmxFile);
    Vector<TMXObjectGroup*> groups = mapInfo->getObjectGroups();  // 获取解析出的groups
    if(!mapInfo)
        return false;
    mapWidth = mapInfo->getMapSize().width;
    mapHeight = mapInfo->getMapSize().height;
    auto  locTilesets = this->mapInfo->getTilesets();
    if(locTilesets.size() == 0)
    {
        cocos2d::log(" Map not found. Please check the filename.");
        return false;
    }
    this->initMapLayer(this->mapInfo);
    CCLOG("layers finished init");
    this->initGroupsInfo(groups);
    
    return true;

}
void StageInfo::initMapLayer(TMXMapInfo *mapInfo)
{
    this->tileSize = mapInfo->getTileSize();
    auto& layers = mapInfo->getLayers();
    if(layers.size()>0)
    {
        TMXLayerInfo* layerInfo = nullptr;
        for (int i = 0; i < layers.size(); i++)
        {
            layerInfo = layers.at(i);
            if(layerInfo)
            {
                vector<BackGroundSprite*> layer;  //此layer中后续用于存储解析的每一层的瓦片
                this->tilesetForLayer(layerInfo, mapInfo,i,layer);  // 解析好每一层存入自定义层中
                
                this->setMyLayers(layer);  // 将自定义层 存入自定义层数组
            }
            
        }
    }
}
#pragma Mark 解析出每一层中瓦片的详细信息  并将其 添加到 _layers 中用于后面自定义绘制地图
void StageInfo::tilesetForLayer(cocos2d::TMXLayerInfo *layerInfo, cocos2d::TMXMapInfo *mapInfo, int layerNum,vector<BackGroundSprite*> &layer)
{
    Size size = layerInfo->_layerSize;
    auto& tilesets = mapInfo->getTilesets();
    if (tilesets.size()>0)
    {
        TMXTilesetInfo* tileset = nullptr;
        for (auto iter = tilesets.crbegin(); iter != tilesets.crend(); ++iter)
        {
            tileset = *iter;
            this->image = tileset->_sourceImage;  // 获取 图片的路径信息
            CCLOG("+++++++++++++++%s",this->image.c_str());
            
            if (!Director::getInstance()->getTextureCache()->getTextureForKey("test"))
            {
                Image testimg ;
                testimg.initWithImageFile(tileset->_sourceImage);
                this->texture = Director::getInstance()->getTextureCache()->addImage(&testimg,"test");//开启即在每个myTile 里面储存 texture
            }
            if (!isInit)
            {
                spacing = tileset->_spacing;
                margin = tileset->_margin;
                firstGid = tileset->_firstGid;
            }
            
            this->_imageSize = this->texture->getContentSizeInPixels();
            if (tileset)
            {
                for( int y=0; y < size.height; y++ )
                {
                    for( int x=0; x < size.width; x++ )
                    {
                        int pos = static_cast<int>(x + size.width * y);
                        int gid = layerInfo->_tiles[ pos ];
                        
                        // gid are stored in little endian.
                        // if host is big endian, then swap
                        //if( o == CFByteOrderBigEndian )
                        //    gid = CFSwapInt32( gid );
                        /* We support little endian.*/
                        
                        // FIXME:: gid == 0 --> empty tile
                        if( gid != 0 )
                        {
                            // Optimization: quick return
                            // if the layer is invalid (more than 1 tileset per layer) an CCAssert will be thrown later
                            if( (gid & kTMXFlippedMask) >= tileset->_firstGid )
                            {
                                 Rect rec = this->getRectForGID(gid, tileset->_margin, tileset->_spacing, tileset->_firstGid);
                                 this->formatTileSetsInfo(tileset, x, y, &rec, layerNum,layer);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    // If all the tiles are 0, return empty tileset
        CCLOG("cocos2d: Warning: TMX Layer '%s' has no tiles", layerInfo->_name.c_str());

}
void StageInfo::formatTileSetsInfo(TMXTilesetInfo *tile, int x, int y, cocos2d::Rect *rect, int layerNum, vector<BackGroundSprite*> &layer)
{
    BackGroundSprite * tileset = new BackGroundSprite();
    tileset->_texture = this->texture;
    tileset->_anchorPoint = Vec2::UNIT_Y;
    tileset->_rect = *rect;
    Vec2 mapPostion = Vec2(tile->_tileSize.width * x, tile->_tileSize.height*y);  // tile 在地图编辑器中的位置
    Vec2 wordPosition = this->transformToWordPos(mapPostion, tile->_tileSize.height);  //转化为游戏中的坐标   原点（0，0）；
    tileset->_postion = wordPosition;
    //_layers.at(layerNum)->push_back(tileset);
    layer.push_back(tileset);
}
Vec2 StageInfo::transformToWordPos(cocos2d::Vec2 &pos, int tileHeight)
{
    return Vec2(pos.x,this->mapHeight * tileHeight - pos.y);
}
Rect StageInfo::getRectForGID(int gid, int margin, int spacing, int firstGid)
{
    Rect rect;
    rect.size = tileSize;
    gid &= kTMXFlippedMask;
    gid = gid - firstGid;
    // max_x means the column count in tile map
    // in the origin:
    // max_x = (int)((_imageSize.width - _margin*2 + _spacing) / (_tileSize.width + _spacing));
    // but in editor "Tiled", _margin variable only effect the left side
    // for compatible with "Tiled", change the max_x calculation
    int max_x = (int)((_imageSize.width - margin + spacing) / (tileSize.width + spacing));
    
    rect.origin.x = (gid % max_x) * (tileSize.width + spacing) + margin;
    rect.origin.y = (gid / max_x) * (tileSize.height + spacing) + margin;
    return rect;
    
}


#pragma mark 设置和获取自定义图层
vector<vector<BackGroundSprite*>>* StageInfo::getStageLayers()
{
    return &_layers;
}
void StageInfo::setMyLayers(vector<BackGroundSprite*> layer)
{
    _layers.push_back(layer);
}


#pragma mark 解析逻辑对象判断层
void StageInfo::initGroupsInfo(Vector<cocos2d::TMXObjectGroup *> groups)
{
    for (auto iter = groups.begin(); iter != groups.end(); ++iter)
    {
        int GROUP_TYPE = 0; //表示当前group数组中的元素属于哪个类型  1001:polygon  1002:polyline  1003:items
        
        TMXObjectGroup * Group = *iter;
        ValueVector currentGroup = Group->getObjects();
        for (int j = 0; j < currentGroup.size();j++)
        {
            ValueMap tempGroup = currentGroup.at(j).asValueMap();  //获取当前group 数组中的一个元素判断其包含的属性
            for(auto iter1 =  tempGroup.begin(); iter1 != tempGroup.end(); iter1++)
            {
                string str1 = iter1->first.c_str();
//                CCLOG("22222222222222222%s",str1.c_str());
                if (str1.compare("points") == 0)
                {
                    GROUP_TYPE = 1001;
                    CCLOG("-----------------------points");
                    break;
                }
                else if(str1.compare("polylinePoints") == 0)
                {
                    GROUP_TYPE = 1002;
                    CCLOG("-----------------------polylinePoints");
                    break;
                }
                else if(j == (int)currentGroup.size() - 1)  //判断完全整个group组的 元素都不包含上面两个属性
                {
                    GROUP_TYPE = 1003;
                    CCLOG("-----------------------items");
                    break;
                }
               // CCLOG("+++++++++++++%s",obj2->first.c_str());
            }
            if(GROUP_TYPE != 0)
            {
                // 一旦赋值 即判断出类型后 循环判断结束
                break;
            }
        }

        vector<ItemGroups*> Items; // 存储items层
        vector<PolygonGroups*> Polygons;//存储多边形层
        vector<PolylineGroups*> Polylines;
        for (auto obj = currentGroup.begin(); obj != currentGroup.end(); ++obj)
        {
            ItemGroups * itemObj = new ItemGroups();
            PolygonGroups * polygonObj = new PolygonGroups();
            PolylineGroups * polylineObj = new PolylineGroups();
            string name = Group->getGroupName(); // 当前group层的 name 目前没有保存
            ValueMap tempMap = obj->asValueMap();
            if(tempMap.find("width") != tempMap.end())
            {
                Value a = tempMap["width"];
                int ab = a.asInt();
                if(GROUP_TYPE == 1001)
                {
                    polygonObj->height = tempMap["height"].asInt();
                    polygonObj->width = tempMap["width"].asInt();
                }
                else if(GROUP_TYPE == 1002)
                {
                    polylineObj->height = tempMap["height"].asInt();
                    polylineObj->width = tempMap["width"].asInt();
                }
                else if(GROUP_TYPE == 1003)
                {
                    itemObj->height = tempMap["height"].asInt();
                    itemObj->width = tempMap["width"].asInt();
                }
                CCLOG("************-%d----, type ==%s",ab,tempMap["type"].asString().c_str());
            }
            if (tempMap.find("type") != tempMap.end())
            {
                if(GROUP_TYPE == 1001)
                {
                    polygonObj->type = tempMap["type"].asInt();
                    polygonObj->name = tempMap["name"].asString();
                    CCLOG("1001");
                }
                else if(GROUP_TYPE == 1002)
                {
                    polylineObj->type = tempMap["type"].asInt();
                    polylineObj->name = tempMap["name"].asString();
                    CCLOG("1002");
                }
                else if(GROUP_TYPE == 1003)
                {
                    itemObj->type = tempMap["type"].asInt();
                    itemObj->name = tempMap["name"].asString();
                }

                CCLOG("type == %d",tempMap["type"].asInt());
            }
            if (tempMap.find("x") != tempMap.end())
            {
                if(GROUP_TYPE == 1001)
                {
                    polygonObj->x = tempMap["x"].asInt();
                    polygonObj->y = tempMap["y"].asInt();
                    CCLOG("1001");
                }
                else if(GROUP_TYPE == 1002)
                {
                    polylineObj->x = tempMap["x"].asInt();;
                    polylineObj->y = tempMap["y"].asInt();;
                    CCLOG("1002");
                }
                else if(GROUP_TYPE == 1003)
                {
                    itemObj->x = tempMap["x"].asInt();
                    itemObj->y = tempMap["y"].asInt();
                }
                CCLOG("x == %d",tempMap["x"].asInt());
            }
            if (tempMap.find("points") != tempMap.end())
            {
                vector<Vec2> vecArray;
                ValueVector points = tempMap["points"].asValueVector(); //获取存储point的数组
                for (auto iter = points.begin(); iter != points.end(); ++iter)
                {
                    Vec2 vec;
                    ValueMap point = iter->asValueMap();
                    vec.x = point["x"].asInt();
                    vec.y = point["y"].asInt();
                    vecArray.push_back(vec);
                    CCLOG("point x = %d  point y = %d ",point["x"].asInt(),point["y"].asInt());
                }
                polygonObj->points.push_back(vecArray);
            }
            if (tempMap.find("polylinePoints") != tempMap.end())
            {
                vector<Vec2> lineArray;
                ValueVector polylines = tempMap["polylinePoints"].asValueVector(); //获取存储point的数组
                for (auto iter = polylines.begin(); iter != polylines.end(); ++iter)
                {
                    Vec2 poly;
                    ValueMap line = iter->asValueMap();
                    poly.x = line["x"].asInt();
                    poly.y = line["y"].asInt();
                    lineArray.push_back(poly);
                    CCLOG("polylinePoints x = %d  polylinePoints y = %d ",line["x"].asInt(),line["y"].asInt());
                }
                if(GROUP_TYPE == 1001)
                    polygonObj->points.push_back(lineArray);
                if (GROUP_TYPE == 1002)
                    polylineObj->points.push_back(lineArray);
            }
            if(tempMap.find("gid") != tempMap.end())  // 对象层 为瓦片items时 获取其rect
            {
                int gid = tempMap["gid"].asInt();
                CCLOG("gid == %d",tempMap["gid"].asInt());
                if(gid != 0)
                {
                    Rect rec = this->getRectForGID(gid, margin,spacing,firstGid);
                    itemObj->rect = rec;
                   // CCLOG("&&&&&&&&&&&&&x = %f, y = %f , w = %f, h = %f",rec.origin.x,rec.origin.y,rec.size.width,rec.size.height);
                }
            }
            for(auto obj2 =  tempMap.begin();obj2!=tempMap.end();obj2++)
            {
               // CCLOG("+++++++++++++%s",obj2->first.c_str());  //打印 容器中的key
            }
            if(GROUP_TYPE == 1001)
            {
                Polygons.push_back(polygonObj);
            }
            else if(GROUP_TYPE == 1002)
            {
                Polylines.push_back(polylineObj);
                CCLOG("%%%%%%%%%%%%%%1002");
            }
            else if(GROUP_TYPE == 1003)
            {
                Items.push_back(itemObj);
            }
            else
            {
                CCLOG("GROUP_TYPE类型错误");
            }
        }
        if (GROUP_TYPE == 1001)
        {
            _PolygonGroups.push_back(Polygons);
        }
        else if (GROUP_TYPE == 1002)
        {
            _PolylineGroups.push_back(Polylines);
            CCLOG("");
        }
        else if(GROUP_TYPE == 1003)
        {
            _ItemsGroups.push_back(Items);
        }
        else
        {
            CCLOG("GROUP_TYPE类型错误");
        }
    }
}
vector<vector<ItemGroups*>>* StageInfo::getItemsGroups()
{
    return &_ItemsGroups;
}
vector<vector<PolygonGroups*>>* StageInfo::getPolygonGroups()
{
    return &_PolygonGroups;
}
vector<vector<PolylineGroups*>>* StageInfo::getPolylineGroups()
{
    return &_PolylineGroups;
}


