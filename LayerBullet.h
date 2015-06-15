#ifndef __LayerBullet_H_
#define __LayerBullet_H_
#include "cocos2d.h"


class LayerBullet : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerBullet);
	bool init();

	void startShoot();
	void stopShoot();
	void addBulletCallback(float dt);
	void bulletMoveFinished(cocos2d::Node * node);
	void removeBullet(cocos2d::Sprite * bullet);

	cocos2d::Vector<cocos2d::Sprite *> _bulletVector;
	//cocos2d::SpriteBatchNode * _bulletBathNode;
};

#endif
