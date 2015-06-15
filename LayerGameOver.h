#ifndef __LayerGameOver_H_
#define __LayerGameOver_H_
#include "cocos2d.h"


class LayerGameOver : public cocos2d::Layer
{
public:
	//CREATE_FUNC(LayerGameOver);
	static LayerGameOver * create(int score);
	static cocos2d::Scene * scene(int score);
	bool init(int score);

	void addBackground();
	void menuBackCallback();
	void menuExitCallback();
	void changeHighestScore(cocos2d::Node * node);
	
public:
	static int _hightestScore;//用于存到本地
	cocos2d::Label * hightestScore;//用于显示
};

#endif
