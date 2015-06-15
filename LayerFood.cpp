#include "LayerFood.h"
#include "AppMacros.h"
USING_NS_CC;

bool LayerFood::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_bigBoomVec = Vector<Sprite *>();//初始化
	_multiBulletsVec = Vector<Sprite *>();

	this->schedule(schedule_selector(LayerFood::addBigBoom),20.0f);
	this->schedule(schedule_selector(LayerFood::addMultiBullets),15.0f);
	return true;
}

void LayerFood::addBigBoom(float dt)
{
	Sprite * bigBoom = Sprite::createWithSpriteFrameName("ufo2.png");
	float minX = bigBoom->getContentSize().width/2;
	float maxX = winSize.width - minX;
	float rangeX = maxX - minX;
	Point boomBrith = Point(minX + rangeX*CCRANDOM_0_1(),winSize.height + bigBoom->getContentSize().height/2);
	bigBoom->setPosition(boomBrith);
	this->addChild(bigBoom);
	_bigBoomVec.pushBack(bigBoom);

	MoveBy * by1 = MoveBy::create(0.5f,Point(0,-150));
	MoveBy * by2 = MoveBy::create(0.3f,Point(0,100));
	MoveBy * by3 = MoveBy::create(1.0f,Point(0,0 - winSize.height - bigBoom->getContentSize().width/2));
	CallFuncN * actionDone = CallFuncN::create(CC_CALLBACK_1(LayerFood::BigBoomMoveFinished,this));
	Sequence * sequence = Sequence::create(by1,by2,by3,actionDone,nullptr);

	bigBoom->runAction(sequence);
}
void LayerFood::BigBoomMoveFinished(cocos2d::Node * pSender)
{
	Sprite * bigBoom = (Sprite *)pSender;
	this->removeChild(bigBoom,true);
	_bigBoomVec.eraseObject(bigBoom);
}
void LayerFood::removeBigBoom(cocos2d::Sprite * boom)
{
	//boom->removeFromParentAndCleanup(true);
	this->removeChild(boom,true);
	_bigBoomVec.eraseObject(boom);
}


void LayerFood::addMultiBullets(float dt)
{
	Sprite * multiBullets = Sprite::createWithSpriteFrameName("ufo1.png");
	float minX = multiBullets->getContentSize().width/2;
	float maxX = winSize.width - minX;
	float rangeX = maxX - minX;
	Point multiBulletsBrith = Point(minX + rangeX*CCRANDOM_0_1(),winSize.height + multiBullets->getContentSize().height/2);
	multiBullets->setPosition(multiBulletsBrith);
	this->addChild(multiBullets);
	_multiBulletsVec.pushBack(multiBullets);

	MoveBy * by1 = MoveBy::create(0.5f,Point(0,-50));
	MoveBy * by2 = MoveBy::create(0.3f,Point(0,30));
	MoveBy * by3 = MoveBy::create(0.8f,Point(0,0 - winSize.height - multiBullets->getContentSize().width/2));
	CallFuncN * actionDone = CallFuncN::create(CC_CALLBACK_1(LayerFood::multiBulletsMoveFinished,this));
	Sequence * sequence = Sequence::create(by1,by2,by3,actionDone,nullptr);

	multiBullets->runAction(sequence);
}
void LayerFood::multiBulletsMoveFinished(cocos2d::Node * pSender)
{
	Sprite * multiBullets = (Sprite *)pSender;
	this->removeChild(multiBullets,true);
	_multiBulletsVec.eraseObject(multiBullets);
}
void LayerFood::removeMultiBullets(cocos2d::Sprite * multiBullets)
{
	this->removeChild(multiBullets,true);
	_multiBulletsVec.eraseObject(multiBullets);
}
