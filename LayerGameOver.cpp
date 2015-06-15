#include "LayerGameOver.h"
#include "SimpleAudioEngine.h"
#include "LayerGameStart.h"
#include "AppMacros.h"
#include "LayerGameMain.h"
using namespace CocosDenshion;
USING_NS_CC;

int LayerGameOver::_hightestScore = 0;

LayerGameOver * LayerGameOver::create(int score)
{
	LayerGameOver * pRet = new LayerGameOver();
	if (pRet && pRet->init(score))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

Scene * LayerGameOver::scene(int score)
{
	Scene * scene = Scene::create();
	LayerGameOver * layer = LayerGameOver::create(score);
	scene->addChild(layer);
	return scene;
}

bool LayerGameOver::init(int score)
{
	if (!Layer::init())
	{
		return false;
	}
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		//如果背景音乐还在播放，则停止其播放
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
	SimpleAudioEngine::getInstance()->playEffect("sound/game_over.wav");

	this->addBackground();

	//添加返回按钮
	Sprite * norBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
	Sprite * pressBackToGame = Sprite::createWithSpriteFrameName("btn_finish.png");
	MenuItemSprite * backItem = MenuItemSprite::create(norBackToGame,
													   pressBackToGame,
													   CC_CALLBACK_0(LayerGameOver::menuBackCallback,this));
	backItem->setPosition(Point(norBackToGame->getContentSize().width/2+10,norBackToGame->getContentSize().height/2+10));
	
	//添加退出按钮
	Sprite * norExit = Sprite::create("CloseNormal.png");
	Sprite * selExit = Sprite::create("CloseSelected.png");
	MenuItemSprite * exitItem = MenuItemSprite::create(norExit,
													   selExit,
													   CC_CALLBACK_0(LayerGameOver::menuExitCallback,this));
	exitItem->setPosition(Point(winSize.width - norExit->getContentSize().width/2-10,norExit->getContentSize().height/2+10));
	Menu * menu = Menu::create(backItem,exitItem,nullptr);
	menu->setPosition(Point::ZERO);
	this->addChild(menu);

	//显示本局分数
	Value strScore(score);
	auto finalScore = Label::createWithBMFont("font/font.fnt",strScore.asString());
	finalScore->setColor(Color3B(0,255,64));
	finalScore->setPosition(Point(winSize.width/2,winSize.height/2));
	this->addChild(finalScore);

	auto delay = DelayTime::create(2.0);
	auto scalebig = ScaleTo::create(1.0, 3.0f);
	auto scalelittle = ScaleTo::create(0.3, 2.0f);
	auto sequence = Sequence::create(delay, scalebig, scalelittle, nullptr);
	finalScore->runAction(sequence);

	//显示历史最高分
	Value strHightestScore(_hightestScore);
	hightestScore = Label::createWithBMFont("font/font.fnt",strHightestScore.asString());
	hightestScore->setColor(Color3B(30,50,240));
	hightestScore->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
	hightestScore->setPosition(Point(150,winSize.height-40));
	this->addChild(hightestScore);

	//更新历史最高分
	if (score > _hightestScore)
	{
		//将最最高分存入到本地文件
		UserDefault::getInstance()->setIntegerForKey("HightestScore",score);
		_hightestScore = score;
		auto delayChange = DelayTime::create(1.3f);
		auto moveOut = MoveBy::create(0.1, Point(0,100));
		auto beginChange = CallFuncN::create(CC_CALLBACK_1(LayerGameOver::changeHighestScore, this));
		auto moveIn = MoveBy::create(0.1,Point(0,-100));
		auto sequence = Sequence::create(delayChange, moveOut, beginChange, moveIn, nullptr);
		hightestScore->runAction(sequence);
	}

	return true;
}

void LayerGameOver::addBackground()
{
	Sprite * bg = Sprite::createWithSpriteFrameName("gameover.png");
	bg->setAnchorPoint(Point::ZERO);
	bg->setPosition(Point(0,-30));//本来应该是Point(0,0)的，为了显示图片最上面的文字，把图片下移了30px
	this->addChild(bg);
}
void LayerGameOver::menuBackCallback()
{
	auto scene = LayerGameStart::scene();
	Director::getInstance()->replaceScene(scene);
}

void LayerGameOver::menuExitCallback()
{
	exit(1);
}

void LayerGameOver::changeHighestScore(Node * node)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/achievement.wav");
	Value changeScore(_hightestScore);
	hightestScore->setString(changeScore.asString());
}
