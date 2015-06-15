#ifndef __LayerMultiBullets_H_
#define __LayerMultiBullets_H_
#include "cocos2d.h"


class LayerMultiBullets : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerMultiBullets);
	bool init();

	void startShoot();
	void stopShoot();
	void addMultiBulletsCallback(float dt);
	void multiBulletsMoveFinished(cocos2d::Node * node);
	void removeMultiBullets(cocos2d::Sprite * bullet);

public:
	cocos2d::Vector<cocos2d::Sprite *> _multiBulletsVector;
	//cocos2d::SpriteBatchNode * _multiBulletsBathNode;
};

#endif
