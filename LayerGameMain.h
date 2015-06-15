#ifndef __LayerGameMain_H_
#define __LayerGameMain_H_
#include "cocos2d.h"
#include "LayerBullet.h"
#include "LayerMultiBullets.h"
#include "MyPlane.h"
#include "LayerEnemy.h"
#include "LayerControl.h"
#include "LayerFood.h"
#include "LayerGameOver.h"

const int  SMALL_SCORE = 5;
const int  MID_SCORE = 30;
const int  BIG_SCORE = 300;

const int MAX_BIGBOOM_COUNT = 100000;
const int TAG_BIGBOOM = 1000;//炸弹图片的标签
const int TAG_BIGBOOMCOUNT = 2000;//炸弹数目（其实是一个菜单）的标签

class LayerGameMain : public cocos2d::Layer
{
public:
	CREATE_FUNC(LayerGameMain);
	static cocos2d::Scene * scene();
	bool init();

	//加载主场景背景
	void addBackground();
	//背景动起来(自定义的回调函数)
	void movingBackgroundCallback(float dt);
	enum BACKGROUND
	{
		BG1,BG2
	};
	//添加飞机
	void addMyPlane();
	//添加子弹层
	void addBulletLayer();
	//添加多重子弹
	void addMultiBulletsLayer();
	//添加敌机
	void addEnemyLayer();
	//添加控制层
	void addControlLayer();
	//添加道具层
	void addFoodLayer();
	//碰撞检测
	void update(float dt);
	//更新大炸弹的函数
	void updateBigBoomCount(int bigBoomCount);
	//点击炸弹回调的函数
	void boomMenuCallback(cocos2d::Ref * ref);

	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
	virtual void onTouchesMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);

public:
	LayerBullet * _bulletLayer;
	LayerMultiBullets * _multiBulletsLayer;
	LayerEnemy * _enemyLayer;
	LayerControl * _controlLayer;
	LayerFood * _foodLayer;


	int _score;
	int _bigBoomCount;
};

#endif
