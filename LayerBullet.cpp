#include "LayerBullet.h"
#include "MyPlane.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

bool LayerBullet::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//_bulletVector = Vector<Sprite *>();//Vector不需要new

	//_bulletBathNode = SpriteBatchNode::create("ui/shoot.png");//使用SpriteBathNode来优化
	//this->addChild(_bulletBathNode);

	this->startShoot();
	return true;
}

void LayerBullet::startShoot()
{
	//开启定时器，设置子弹发射的时间（每隔0.3秒发射一个子弹）
	this->schedule(schedule_selector(LayerBullet::addBulletCallback),0.25f);
}
void LayerBullet::stopShoot()
{
	//关闭定时器
	this->unschedule(schedule_selector(LayerBullet::addBulletCallback));
}

void LayerBullet::addBulletCallback(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/bullet.wav");
	//在另外一个层里获得飞机的单例
	Sprite * plane = MyPlane::getInstance();
	//设置子弹发射的位置（飞机头处）
	Point birthPlace = plane->getPosition() + Point(0,plane->getBoundingBox().size.height/2 + 5);
	//添加子弹
	Sprite * bullet = Sprite::createWithSpriteFrameName("bullet1.png");
	//设置子弹位置
	bullet->setPosition(birthPlace);
	//将子弹加入到_bulletBathNode（为了优化）
	//_bulletBathNode->addChild(bullet);
	this->addChild(bullet);
	//将子弹加入到_bulletVector，方便后面的操作
	_bulletVector.pushBack(bullet);
	//_bulletArray->addObject(bullet);
	//得到子弹到屏幕上边沿的距离
	float distance = winSize.height - plane->getPositionY() - plane->getBoundingBox().size.height/2;
	//确定子弹的速度 一秒跨越320个像素。
	float velocity = 800/1;
	//根据距离和速率求得时间
	float movedt = distance / velocity;
	//子弹在movedt的时间内移动到屏幕上边沿之外的地方（加上的 bullet->getContentSize().height 就是超出屏幕的距离）
	MoveTo * to = MoveTo::create(movedt,Point(birthPlace.x,winSize.height + bullet->getContentSize().height));
	//当动作完成后（子弹撞击到敌机或者飞入天际。）就将子弹移除
	CallFuncN * actionDone = CallFuncN::create(this,callfuncN_selector(LayerBullet::bulletMoveFinished));
	Sequence * sequence = Sequence::create(to,actionDone,NULL);
	bullet->runAction(sequence);
}
//子弹移动完成后的函数
void LayerBullet::bulletMoveFinished(cocos2d::Node * node)
{
	Sprite * spr = (Sprite *)node;
	//将子弹从_bulletBathNode移除
	//_bulletBathNode->removeChild(spr,true);
	this->removeChild(spr,true);
	//将子弹从_bulletVector中移除
	_bulletVector.eraseObject(spr);
	//_bulletArray->removeObject(spr);
}

//去除子弹
void LayerBullet::removeBullet(Sprite * bullet)
{
	if (bullet != nullptr)
	{
		//_bulletBathNode->removeChild(bullet,true);//3.0之后不建议使用
		this->removeChild(bullet,true);
		_bulletVector.eraseObject(bullet);
	}
}
