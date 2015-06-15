#ifndef __LayerControl_H_
#define __LayerControl_H_
#include "cocos2d.h"
#include "LayerNoTouch.h"
class LayerControl : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerControl);
	bool init();

	void menuCallback(cocos2d::Ref * ref);
	void updateScore(int score);
	

private:
	cocos2d::MenuItemSprite * pauseMenuItem;
	cocos2d::LabelBMFont * scoreItem;
	LayerNoTouch * _noTouchLayer;
};

#endif
