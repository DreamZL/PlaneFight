#ifndef __LayerGameStart_H_
#define __LayerGameStart_H_
#include "cocos2d.h"

class LayerGameStart : public cocos2d::Layer
{
public:
	static cocos2d::Scene * scene();
	bool init();
	CREATE_FUNC(LayerGameStart);

	void addStartGamePicture();
	void addPerloadMusic();
	bool isSaveFile();
	void getHightestScore();

	void toMainGameCallback();
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
};

#endif
