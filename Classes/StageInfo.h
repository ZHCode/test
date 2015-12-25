//
//  ParseTmxMap.h
//  cocos2dXTest
//
//  Created by 1 on 15/12/18.
//
//

#ifndef StageInfo_h
#define StageInfo_h
#include "cocos2d.h"
//#include "2d/CCNode.h"
//#include "2d/CCTMXObjectGroup.h"
//#include "base/CCValue.h"
#include "BackGroundSprite.h"
#include "ItemGroups.h"
#include "PolygonGroups.h"
#include "PolylineGroups.h"
USING_NS_CC;
using namespace std;
class StageInfo : public Node
{
protected:
    
    int margin ; //地图瓦片间的缝隙
    int spacing;
    int firstGid; //确定第一个瓷砖的位置
    bool isInit; //标记上面三个值是否都经过首次初始化  //三个值会 影响到group 层的items 算取rect 截取纹理切图
    vector<vector<BackGroundSprite*>> _layers;  // 储存地图的每一层  每一层中储存 每一层的瓦片信息

    vector<vector<ItemGroups*>> _ItemsGroups;  // items 层
    vector<vector<PolygonGroups*>> _PolygonGroups;//   多边形 层
    vector<vector<PolylineGroups*>> _PolylineGroups;//    折线 层
public:
    
    static StageInfo * create(const string& tmxFile);
    virtual bool initTmxMapInfo(const string& tmxFile);
    void initMapLayer(TMXMapInfo * mapInfo);
    void initGroupsInfo(Vector<TMXObjectGroup*> groups);
    void tilesetForLayer(TMXLayerInfo *layerInfo, TMXMapInfo *mapInfo,int layerNum,vector<BackGroundSprite*> &layer);
    void formatTileSetsInfo(TMXTilesetInfo* tile,int x,int y, Rect* rect,int layerNum,vector<BackGroundSprite*> &layer);
    Vec2 transformToWordPos(Vec2& pos, int tileHeight);
    Rect getRectForGID(int gid,int margin,int spacing,int firstGid);  //当前layer 瓦片所用的贴图的rect
    
    void setMyLayers(vector<BackGroundSprite*> layer);//  解析出来的存储着每层瓦片的 自定义层
    
   
    Size mapSize;   // 地图大小  单位块
    int mapHeight;  // 地图高  单位块
    int mapWidth;   // 地图宽  单位块
    string image;   // 地图所用贴图
    Texture2D * texture;  //地图贴图纹理
    TMXMapInfo * mapInfo;  //tmx 读取的地图信息
    bool isHasPoints; //标记对象层是否有多边形逻辑判担区域
    Size tileSize; // 瓦片的大小 单位像素
    Size _imageSize;// 地图所用的图片大小
    
    
    // 获取 解析出自定义层 数组
    vector<vector<BackGroundSprite*>>* getStageLayers();
    
    //gropus层解析数据
    vector<vector<ItemGroups*>>* getItemsGroups();
    
    vector<vector<PolygonGroups*>>* getPolygonGroups();
    
    vector<vector<PolylineGroups*>>* getPolylineGroups();
    
    
    
    
    
};

#endif /* ParseTmxMap_h */
