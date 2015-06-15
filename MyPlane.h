#ifndef __Plane_H_
#define __Plane_H_
#include "cocos2d.h"

class MyPlane : public cocos2d::Sprite
{
public:
	//将飞机设计成全局的
	static MyPlane * getInstance();
	static MyPlane * _splane;

	bool init();
	
	//玩家飞机在正常时是动画的，不是一张单一的图片
	void PlanAnimate();

	//玩家飞机爆炸的动画
	void PlaneBlowup(int score);

	//删除玩家飞机
	void removePlane();

	int score;

};

#endif
