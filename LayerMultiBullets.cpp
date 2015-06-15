#include "LayerMultiBullets.h"
#include "AppMacros.h"
#include "MyPlane.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

bool LayerMultiBullets::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//_multiBulletsBathNode = SpriteBatchNode::create("ui/shoot.png");
	//this->addChild(_multiBulletsBathNode);

	_multiBulletsVector = Vector<Sprite *>();
	//this->startShoot();

	return true;
}
void LayerMultiBullets::startShoot()
{
	this->schedule(schedule_selector(LayerMultiBullets::addMultiBulletsCallback),0.2f);
}
void LayerMultiBullets::stopShoot()
{
	this->unschedule(schedule_selector(LayerMultiBullets::addMultiBulletsCallback));
}
void LayerMultiBullets::addMultiBulletsCallback(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
	Sprite * plane = MyPlane::getInstance();
	Point birthPlaceLeft = plane->getPosition() + Point(-10,plane->getBoundingBox().size.height/2 + 5);
	Point birthPlaceRight = plane->getPosition() + Point(+10,plane->getBoundingBox().size.height/2 + 5);
	Sprite * bulletLeft = Sprite::createWithSpriteFrameName("bullet2.png");
	Sprite * bulletRight = Sprite::createWithSpriteFrameName("bullet2.png");
	bulletLeft->setPosition(birthPlaceLeft);
	bulletRight->setPosition(birthPlaceRight);
	//_multiBulletsBathNode->addChild(bulletLeft);
	//_multiBulletsBathNode->addChild(bulletRight);
	this->addChild(bulletLeft);
	this->addChild(bulletRight);
	_multiBulletsVector.pushBack(bulletLeft);
	_multiBulletsVector.pushBack(bulletRight);
	float distance = winSize.height - plane->getPositionY() - plane->getBoundingBox().size.height/2;
	float velocity = 800/1;
	float movedt = distance / velocity;
	MoveTo * to1 = MoveTo::create(movedt,Point(birthPlaceLeft.x,winSize.height + bulletLeft->getContentSize().height));
	MoveTo * to2 = MoveTo::create(movedt,Point(birthPlaceRight.x,winSize.height + bulletRight->getContentSize().height));
	CallFuncN * actionDone1 = CallFuncN::create(this,callfuncN_selector(LayerMultiBullets::multiBulletsMoveFinished));
	CallFuncN * actionDone2 = CallFuncN::create(this,callfuncN_selector(LayerMultiBullets::multiBulletsMoveFinished));
	Sequence * sequence1 = Sequence::create(to1,actionDone1,nullptr);
	Sequence * sequence2 = Sequence::create(to2,actionDone2,nullptr);
	bulletLeft->runAction(sequence1);
	bulletRight->runAction(sequence2);
}
void LayerMultiBullets::multiBulletsMoveFinished(cocos2d::Node * node)
{
	Sprite * spr = (Sprite *)node;
	//_multiBulletsBathNode->removeChild(spr,true);
	this->removeChild(spr,true);
	_multiBulletsVector.eraseObject(spr);
}
void LayerMultiBullets::removeMultiBullets(cocos2d::Sprite * bullet)
{
	//_multiBulletsBathNode->removeChild(bullet,true);
	this->removeChild(bullet,true);
	_multiBulletsVector.eraseObject(bullet);
}
