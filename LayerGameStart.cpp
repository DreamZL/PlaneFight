#include "LayerGameStart.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
#include "LayerGameMain.h"
#include "LayerGameOver.h"
using namespace CocosDenshion;
USING_NS_CC;

//定义一个宏
#define loadMusic() CocosDenshion::SimpleAudioEngine::getInstance()

Scene * LayerGameStart::scene()
{
	auto scene = Scene::create();
	auto layer = LayerGameStart::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameStart::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->addPerloadMusic();
	this->addStartGamePicture();
	this->getHightestScore();

	return true;
}

void LayerGameStart::addStartGamePicture()
{
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav");
	//设置键盘响应事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(LayerGameStart::onKeyPressed,this);
	listener->onKeyReleased = CC_CALLBACK_2(LayerGameStart::onKeyReleased,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//加载全局图片信息,这是个单例，没有挂在渲染树上，所以当场景切换的时候它依然存在
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot_background.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ui/shoot.plist");

	//加载当前gamestart页面所需要的图片，动画
	auto background = Sprite::createWithSpriteFrameName("background.png");
	background->setAnchorPoint(Point(0,0));
	background->setPosition(Point(0,0));
	this->addChild(background);

	auto copyright = Sprite::createWithSpriteFrameName("shoot_copyright.png");
	copyright->setAnchorPoint(Point(0.5,0));
	copyright->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(copyright);

	auto loading = Sprite::createWithSpriteFrameName("game_loading1.png");
	loading->setPosition(Point(winSize.width/2,winSize.height/2-40));
	this->addChild(loading);

	//创建动画
	auto animation = Animation::create();
	animation->setDelayPerUnit(0.2);
	for (int i = 1; i <= 4; i++)
	{
		auto frame = SpriteFrame::create(StringUtils::format("shoot_background/game_loading%d.png",i),Rect(0,0,186,38));
		animation->addSpriteFrame(frame);
	}
	auto animate = Animate::create(animation);
	auto repeat = Repeat::create(animate,4);//动画播放四次
	auto loadingDone = CallFunc::create(this,callfunc_selector(LayerGameStart::toMainGameCallback));
	auto sequence = Sequence::create(repeat,loadingDone,NULL);
	loading->runAction(sequence);

}
void LayerGameStart::addPerloadMusic()
{
	//加载全局音乐信息
	loadMusic()->preloadBackgroundMusic("sound/game_music.wav");
	loadMusic()->preloadEffect("sound/achievement.wav");
	loadMusic()->preloadEffect("sound/big_spaceship_flying.wav");
	loadMusic()->preloadEffect("sound/bullet.wav");
	loadMusic()->preloadEffect("sound/button.wav");
	loadMusic()->preloadEffect("sound/enemy1_down.wav");
	loadMusic()->preloadEffect("sound/enemy2_down.wav");
	loadMusic()->preloadEffect("sound/enemy3_down.wav");
	loadMusic()->preloadEffect("sound/game_over.wav");
	loadMusic()->preloadEffect("sound/get_bomb.wav");
	loadMusic()->preloadEffect("sound/get_double_laser.wav");
	loadMusic()->preloadEffect("sound/out_porp.wav");
	loadMusic()->preloadEffect("sound/use_bomb.wav");
}
void LayerGameStart::toMainGameCallback()
{
	auto scene = LayerGameMain::scene();
	Director::getInstance()->replaceScene(scene);
}

//判断分数是否已经被存储
bool LayerGameStart::isSaveFile()
{
	//用一个bool值作为标志，如果有则表示分数已经被存储
	if (!UserDefault::getInstance()->getBoolForKey("isSaveFileXml"))
	{
		UserDefault::getInstance()->setBoolForKey("isSaveFileXml",true);
		UserDefault::getInstance()->setIntegerForKey("HightestScore",0);
		UserDefault::getInstance()->flush();
		return false;
	}
	else
		return true;
}
void LayerGameStart::getHightestScore()
{
	if (isSaveFile())
	{
		//在这里设置历史最高得分
		LayerGameOver::_hightestScore = UserDefault::getInstance()->getIntegerForKey("HightestScore",0);
	}
}

void LayerGameStart::onKeyPressed(EventKeyboard::KeyCode keyCode, Event * event)
{
	log("Key with keycode %d pressed", keyCode);
}

void LayerGameStart::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * event)
{
	log("Key with keycode %d released", keyCode);
}
