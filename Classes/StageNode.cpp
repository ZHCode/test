//
//  StageNode.cpp
//  cocos2dXTest
//
//  Created by 1 on 15/12/23.
//
//

#include "StageNode.h"
const int TAG_SP_SHOW = 100001;// flag the sprite is show
const int TAG_SP_HIDE = 100002;// flag the sprite is hide

StageNode * StageNode::createWithStageInfo(StageInfo *info)
{
//    if(info)
//    {
//        return nullptr;
//    }
    StageNode *ret = new  StageNode();
    if (ret && ret->initWithStageInfo(info))
    {
        ret->autorelease();
        return ret;
    }
    else
        return nullptr;
}
bool StageNode::initWithStageInfo(StageInfo *info)
{
    SHOW_HEIGHT = Director::getInstance()->getWinSize().height;
    //SHOW_HEIGHT = visibleSize.height;
    limitTopHeight = SHOW_HEIGHT*3/2 ;
    limitDownHeight = -SHOW_HEIGHT/2;
    speed = 4;
    MapWidth = info->mapWidth * info->tileSize.width;
    
    //draw  background
    auto layers = info->getStageLayers();
    for (auto layer = layers->begin(); layer != layers->end(); ++layer)
    {
        BackGroundSprite * tileset = nullptr;
        for (auto iter = layer->begin(); iter != layer->end(); ++iter)
        {
            tileset = *iter;
            auto layerSp = Sprite::create();
            layerSp->initWithTexture(tileset->_texture, tileset->_rect);  // 正式的时候用texture
            layerSp->setAnchorPoint(tileset->_anchorPoint);
            layerSp->setPosition(tileset->_postion);
            layerSp->setTag(TAG_SP_HIDE);
            if (tileset->_postion.y < SHOW_HEIGHT*3/2 && tileset->_postion.y > -SHOW_HEIGHT/2)//高于屏幕一半的显示出来且不低于 负1/2的屏幕的不加载
            {
                this->addChild(layerSp);
                layerSp->setTag(TAG_SP_SHOW);
            }
            layerSpriteArray.pushBack(layerSp);
        }
        
    }
    
    //draw logic items
    vector<vector<ItemGroups*>> * items = info->getItemsGroups();
    for (auto item = items->begin(); item != items->end(); item++)
    {
        for (auto itemobj = item->begin(); itemobj != item->end(); itemobj++)
        {
            ItemGroups* obj = *itemobj;
            auto itemSprite = Sprite::create();
            itemSprite->initWithTexture(info->texture, obj->rect);
            itemSprite->setAnchorPoint(Vec2(0, 0));
            itemSprite->setPosition(Vec2(obj->x,obj->y));
            itemSprite->setTag(TAG_SP_HIDE);
            if (itemSprite->getPositionY() < SHOW_HEIGHT*3/2 - itemSprite->getContentSize().height && itemSprite->getPositionY() > -SHOW_HEIGHT/2)//高于屏幕一半的显示出来且不低于 负1/2的屏幕的不加载
            {
                this->addChild(itemSprite);
                itemSprite->setTag(TAG_SP_SHOW);
            }
            ItemsSpriteArray.pushBack(itemSprite);
        }
    }

    
    this->scheduleUpdate();
    return true;
}
void StageNode::update(float dt)
{
    for (auto iter = layerSpriteArray.begin(); iter != layerSpriteArray.end(); ++iter)
    {
        Sprite  *lSp = *iter;
        if (lSp->getPositionY() < limitTopHeight && lSp->getPositionY() > limitDownHeight && lSp->getTag() == TAG_SP_HIDE)
        {
            this->addChild(lSp);
            lSp->setTag(TAG_SP_SHOW);
        }
        if(lSp->getPositionY() < limitDownHeight && lSp->getTag() == TAG_SP_SHOW)
        {
            lSp->setTag(TAG_SP_HIDE);
            lSp->removeFromParent();
        }
        //lSp->runAction(MoveTo::create(0.1, Vec2(lSp->getPositionX(),lSp->getPositionY() - speed)));
       lSp->setPositionY(lSp->getPositionY() - speed);
    }
    
    for (auto item = ItemsSpriteArray.begin(); item != ItemsSpriteArray.end(); item++)
    {
        Sprite *iSprite = *item;
        if(iSprite->getPositionY() < limitTopHeight - iSprite->getContentSize().height && iSprite->getPositionY() > limitDownHeight && iSprite->getTag() == TAG_SP_HIDE)
        {
            this->addChild(iSprite);
            iSprite->setTag(TAG_SP_SHOW);
        }
        if (iSprite->getPositionY() < limitDownHeight && iSprite->getTag() == TAG_SP_SHOW)
        {
            iSprite->setTag(TAG_SP_HIDE);
            iSprite->removeFromParent();
        }
       // iSprite->runAction(MoveTo::create(0.1, Vec2(iSprite->getPositionX(),iSprite->getPositionY() - speed)));
        iSprite->setPositionY(iSprite->getPositionY() - speed);
    }
}
void StageNode::setSpeed(float num)
{
    speed = num;
}
float StageNode::getMapWidth()
{
    return MapWidth;
}