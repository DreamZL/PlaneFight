#ifndef __LayerNoTouch_H_
#define __LayerNoTouch_H_
#include "cocos2d.h"

class LayerNoTouch : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerNoTouch);
	bool init();

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
};

#endif
