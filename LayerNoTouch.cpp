#include "LayerNoTouch.h"
USING_NS_CC;

bool LayerNoTouch::init()
{
	if (!Layer::create())
	{
		return false;
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(LayerNoTouch::onTouchBegan,this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
	//this->_eventDispatcher->setPriority(listener,1); //设置优先级
	listener->setSwallowTouches(true);//吞噬触摸
	return true;
}

bool LayerNoTouch::onTouchBegan(Touch *touch, Event *unused_event)
{
	return true;
}
