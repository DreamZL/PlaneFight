#include "LayerControl.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;

bool LayerControl::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_noTouchLayer = nullptr;//初始化
	//暂停按钮不同状态下的两个图片
	Sprite * nor = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	Sprite * press = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	pauseMenuItem = MenuItemSprite::create(nor,press,CC_CALLBACK_1(LayerControl::menuCallback,this));
	Point menuBrith = Point(pauseMenuItem->getContentSize().width/2 + 10, winSize.height - pauseMenuItem->getContentSize().height);
	pauseMenuItem->setPosition(menuBrith);
	Menu * pauseMenu = Menu::create(pauseMenuItem,nullptr);
	pauseMenu->setPosition(Point::ZERO);
	this->addChild(pauseMenu,101);//将暂停/继续 按钮放在最前面

	scoreItem = LabelBMFont::create("0","font/font.fnt");
	scoreItem->setColor(Color3B(255,255,0));
	scoreItem->setAnchorPoint(Point(0,0.5));
	scoreItem->setPosition(Point(pauseMenuItem->getPositionX() + nor->getContentSize().width/2 + 5, pauseMenuItem->getPositionY()));
	this->addChild(scoreItem);

	return true;
}

//按钮回调函数
void LayerControl::menuCallback(cocos2d::Ref * ref)
{
	if (!Director::getInstance()->isPaused())//如果点击按钮之前游戏没有暂停
	{
		if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			//如果背景音乐还在播放，则暂停其播放
			SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		}
		//则将 暂停/继续 按钮设置为继续状态的按钮
		pauseMenuItem->setNormalImage(Sprite::createWithSpriteFrameName("game_resume_nor.png"));
		pauseMenuItem->setSelectedImage(Sprite::createWithSpriteFrameName("game_resume_pressed.png"));
		//并暂停游戏
		Director::getInstance()->pause();
		//添加屏蔽层
		_noTouchLayer = LayerNoTouch::create();
		this->addChild(_noTouchLayer);
	}
	else
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();//恢复背景音乐
		pauseMenuItem->setNormalImage(Sprite::createWithSpriteFrameName("game_pause_nor.png"));
		pauseMenuItem->setSelectedImage(Sprite::createWithSpriteFrameName("game_pause_pressed.png"));
		Director::getInstance()->resume();
		this->removeChild(_noTouchLayer,true);
	}
}
void LayerControl::updateScore(int score)
{
	/*2.0版本
	String * strScore = String::createWithFormat("%d",score);
	scoreItem->setString(strScore->getCString());*/
	
	//3.0版本
	Value strScore(score);
	scoreItem->setString(strScore.asString());//更新成绩转换为字符串
}

