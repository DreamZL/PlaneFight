#ifndef __LayerFood_H_
#define __LayerFood_H_
#include "cocos2d.h"

class LayerFood : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerFood);
	bool init();

	void addBigBoom(float dt);
	void BigBoomMoveFinished(cocos2d::Node * pSender);
	void removeBigBoom(cocos2d::Sprite * boom);

	void addMultiBullets(float dt);
	void multiBulletsMoveFinished(cocos2d::Node * pSender);
	void removeMultiBullets(cocos2d::Sprite * multiBullets);

public:
	cocos2d::Vector<cocos2d::Sprite *> _bigBoomVec;
	cocos2d::Vector<cocos2d::Sprite *> _multiBulletsVec;
};

#endif
