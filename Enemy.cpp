#include "Enemy.h"
USING_NS_CC;

Enemy::Enemy()
{
	//在构造函数中初始化，其实也可以在init函数中初始化，但这里没有init函数
	_sprite = nullptr;
	_life = 0;
}
Enemy::~Enemy()
{
	
}
Enemy * Enemy::create()
{
	Enemy * pRect = new Enemy();
	if (pRect != nullptr)
	{
		pRect->autorelease();
		return pRect;
	}
	else
		return nullptr;

	
}
void Enemy::bindEnemySprite(cocos2d::Sprite * spr,int life)
{
	_sprite = spr;
	_life = life;
	//将_sprite加到 pRect 上！！pRect 实质就是一个Sprite
	this->addChild(_sprite);
}
Sprite * Enemy::getSprite()
{
	return _sprite;
}
int Enemy::getLife()
{
	return _life;
}
void Enemy::loseLife()
{
	_life--;
}
//自定义的getBoundingBox函数
Rect Enemy::Get_BoundingBox()
{
	Rect rect = _sprite->getBoundingBox();
	//本来敌机是加到pRect上的它的坐标是相对于pRect的
	//这里将敌机的坐标转换为世界坐标
	Point position = this->convertToWorldSpace(rect.origin);
	//这里只需要知道敌机的起始坐标，因为敌机的宽度和长度是不会改变的
	Rect enemyRect = Rect(position.x, position.y, rect.size.width, rect.size.height);
	return enemyRect;
}
