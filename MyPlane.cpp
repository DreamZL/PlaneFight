#include "MyPlane.h"
#include "LayerGameOver.h"
USING_NS_CC;

MyPlane * MyPlane::_splane = nullptr;

MyPlane * MyPlane::getInstance()
{
	if (!_splane)
	{
		_splane = new MyPlane();
		if (_splane && _splane->init())
		{
			//不将其挂到渲染树上，让飞机的生命周期跟场景一样
			//_splane->autorelease();
		}
	}
	return _splane;//return 在if语句外面
}

bool MyPlane::init()
{
	Sprite::init();

	score = 0;
	//用帧动画的帧来加载精灵
	this->initWithSpriteFrameName("hero1.png");
	//一秒闪三次
	Blink * blink = Blink::create(1,3);

	//闪3次以后，飞机执行自身动画
	CallFunc * callFunc = CallFunc::create(this,callfunc_selector(MyPlane::PlanAnimate));
	Sequence * sequence = Sequence::create(blink,callFunc,NULL);//Sequence不能打包repeatforever
	this->runAction(sequence);

	//加载飞机爆炸的动画缓存
	Animation * animation = Animation::create();
	animation->setDelayPerUnit(0.1f);
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n2.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n3.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero_blowup_n4.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero1.png"));//坑！！！
	//给加载的动画取名"PlaneBlowup"
	AnimationCache::getInstance()->addAnimation(animation,"PlaneBlowup");

	return true;
}

//飞机自身的动作
void MyPlane::PlanAnimate()
{
	Animation * animation = Animation::create();
	animation->setDelayPerUnit(0.2f);
	animation->setLoops(-1);
	//因为要加入的图片只有2个所以就不用for循环了
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->spriteFrameByName("hero2.png"));
	Animate * animate = Animate::create(animation);
	this->runAction(animate);
}

//玩家飞机爆炸的动画
void MyPlane::PlaneBlowup(int passScore)
{
	score = passScore;
	//取出前面加载的动画缓存"PlaneBlowup"
	Animate * animate = Animate::create(AnimationCache::getInstance()->animationByName("PlaneBlowup"));
	//在飞机爆炸完毕之后 除去飞机
	CallFunc * actionDone = CallFunc::create(this,callfunc_selector(MyPlane::removePlane));
	Sequence * sequence = Sequence::create(animate,actionDone,NULL);
	this->runAction(sequence);

}

//除去玩家飞机
void MyPlane::removePlane()
{
	//飞机没有autorelease(),它的引用记数为1，再把它加到父类上去引用记数+1变为2
	//所以这里要将他从父类上拿掉引用记数-1变为1。飞机还存在只不过没在渲染树上了
	this->removeFromParent();

	//玩家飞机爆炸，切换游戏结束的场景
	auto pScene = LayerGameOver::scene(score);//有巨坑！！在切换到结束场景后，再返回到主场景再来一次游戏，
	Director::getInstance()->replaceScene(pScene);//玩家飞机不能播放自身动画，而且加到上面的飞机变成了爆炸后的样子
												//为了好看，我就在动画后面的最后一帧加上了飞机正常的图片。。。。

}
