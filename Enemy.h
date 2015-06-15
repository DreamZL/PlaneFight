#ifndef __Enemy_H_
#define __Enemy_H_
#include "cocos2d.h"

class Enemy : public cocos2d::Node
{
public:
	//构造器
	Enemy();
	//析构器
	~Enemy();
	//创建敌机
	static Enemy * create();
	//将敌机与其对应的Sprite（图片）和生命值绑定
	void bindEnemySprite(cocos2d::Sprite * spr,int life);
	//得到敌机
	cocos2d::Sprite * getSprite();
	//得到生命值
	int getLife();
	//失去生命值
	void loseLife();
	//得到敌机在世界坐标内的的位置和尺寸大小boundingbox
	cocos2d::Rect Get_BoundingBox();
	
private:
	cocos2d::Sprite * _sprite;
	int _life;
};

#endif
