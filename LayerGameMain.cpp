#include "LayerGameMain.h"
#include "AppMacros.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC;


Scene * LayerGameMain::scene()
{
	Scene * scene = Scene::create();
	LayerGameMain * layer = LayerGameMain::create();
	scene->addChild(layer);
	return scene;
}

bool LayerGameMain::init()
{
	if (!Layer::init())
	{
		return false;
	}
	_bigBoomCount = 0;
	_score = 0;//不要将_score 设置为static ，否则在场景切换时，它不能清零

	this->addBackground();

	this->addMyPlane();

	this->addBulletLayer();//执行了startShoot()

	this->addMultiBulletsLayer();//没有执行startShoot()

	this->addEnemyLayer();

	this->addFoodLayer();

	this->addControlLayer();

	this->scheduleUpdate();//开启定时器便于碰撞检测

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);//触摸吞噬
	listener->onTouchBegan = CC_CALLBACK_2(LayerGameMain::onTouchBegan,this);
	listener->onTouchMoved = CC_CALLBACK_2(LayerGameMain::onTouchesMoved,this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

//添加背景
void LayerGameMain::addBackground()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("sound/game_music.wav",true);

	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setTag(BG1);
	bg1->setAnchorPoint(Point::ZERO);
	bg1->setPosition(Point(0,0));
	this->addChild(bg1);

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setTag(BG2);
	bg2->setAnchorPoint(Point::ZERO);
	bg2->setPosition(0,bg1->getContentSize().height - 5);
	this->addChild(bg2);
	
	this->schedule(schedule_selector(LayerGameMain::movingBackgroundCallback),0.01f);
}
//使得背景滚动起来
void LayerGameMain::movingBackgroundCallback(float dt)
{
	Sprite * bg1 = (Sprite *)this->getChildByTag(BG1);
	Sprite * bg2 = (Sprite *)this->getChildByTag(BG2);
	
	bg1->setPositionY(bg1->getPositionY() - 2);
	bg2->setPositionY(bg1->getPositionY() + bg2->getContentSize().height - 2);

	if (bg2->getPositionY() < 0)
	{
		bg1->setPositionY(0);
	}
}
//添加飞机
void LayerGameMain::addMyPlane()
{
	Sprite * myplane = MyPlane::getInstance();
	myplane->setPosition(Point(winSize.width/2,myplane->getContentSize().height/2));
	myplane->setScale(0.7f);
	this->addChild(myplane);//飞机引用记数+1
}

//点击回调
bool LayerGameMain::onTouchBegan(Touch *touch, Event *unused_event)
{
	//将bounddingBox扩大，让玩家更容易控制飞机
	Rect planeRect = MyPlane::getInstance()->getBoundingBox();
	planeRect.origin.x -= 40;
	planeRect.origin.y -= 50;
	planeRect.size.width += 80;
	planeRect.size.height += 100;
	if (planeRect.containsPoint(touch->getLocation()))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//移动回调
void LayerGameMain::onTouchesMoved(Touch *touch, Event *unused_event)
{
	MyPlane * myPlane = MyPlane::getInstance();
	Point position = myPlane->getPosition() + touch->getDelta();
	//判断飞机是否飞出屏幕用 或（||）来连接
	if (position.x < myPlane->getBoundingBox().size.width/2 ||
		position.x > winSize.width - myPlane->getBoundingBox().size.width/2 ||
		position.y < myPlane->getBoundingBox().size.height/2 ||
		position.y > winSize.height - myPlane->getBoundingBox().size.height/2)
	{
		return;
	}
	else
		MyPlane::getInstance()->setPosition(position);
}

void LayerGameMain::addBulletLayer()
{
	_bulletLayer = LayerBullet::create();
	this->addChild(_bulletLayer);
}
void LayerGameMain::addMultiBulletsLayer()
{
	_multiBulletsLayer = LayerMultiBullets::create();
	this->addChild(_multiBulletsLayer);
}
void LayerGameMain::addEnemyLayer()
{
	_enemyLayer = LayerEnemy::create();
	this->addChild(_enemyLayer);
}

void LayerGameMain::update(float dt)
{
	//单发子弹与小敌机碰撞
	/*思路：
		两次遍历（即两个for循环），第一次遍历子弹容器（_bulletVector），取出其第一个子弹，
	第二次遍历小敌机容器（_smallVec）将这个取出的子弹与当前屏幕上所有的小敌机做碰撞检测，
	如果检测到碰撞，再判断当前碰撞到的小敌机的生命值_life 若等于1，则小敌机失去生命值
	再分别将当前的子弹和当前的小敌机加到容器 bulletToDel 和 smallToDel 中去，
	当第一个子弹与屏幕上的敌机全部碰撞检测完以后，就把 bulletToDel 和 smallToDel
	里面的对象全部删除*/
	Vector<Sprite *> bulletToDel_Small;
	for (auto bt : _bulletLayer->_bulletVector)
	{
		Sprite * bullet = bt;
		Vector<Enemy *> smallToDel;
		for (auto et : _enemyLayer->_smallVec)
		{
			Enemy * enemy = et;
			if (bullet->getBoundingBox().intersectsRect(enemy->Get_BoundingBox()))
			{
				if (enemy->getLife() == 1)
				{
					enemy->loseLife();
					bulletToDel_Small.pushBack(bullet);
					smallToDel.pushBack(enemy);
					_score += SMALL_SCORE;//加上小敌机的分数
					_controlLayer->updateScore(_score);
				}
			}

		}
		for(auto et : smallToDel)//注意for循环的位置，要与创建时的语句在同一层
		{
			Enemy * enemy = et;
			_enemyLayer->smallEnemyBlowup(enemy);//敌机爆炸（删除）
		}
	}
	for (auto bt : bulletToDel_Small)//注意for循环的位置，要与创建时的语句在同一层
	{
		Sprite * bullet = bt;
		_bulletLayer->removeBullet(bullet);//删除子弹
	}

	//单发子弹与中敌机碰撞
	Vector<Sprite *> bulletToDel_Mid;
	for (auto bt : _bulletLayer->_bulletVector)
	{
		Sprite * bullet = bt;
		Vector<Enemy *> midToDel;
		for (auto et : _enemyLayer->_midVec)
		{
			Enemy * enemyMid = et;
			if (bullet->getBoundingBox().intersectsRect(enemyMid->Get_BoundingBox()))
			{
				if (enemyMid->getLife() > 1)
				{
					enemyMid->loseLife();
					bulletToDel_Mid.pushBack(bullet);
				}
				if (enemyMid->getLife() == 1)
				{
					enemyMid->loseLife();
					bulletToDel_Mid.pushBack(bullet);
					midToDel.pushBack(enemyMid);
					_score += MID_SCORE;
					_controlLayer->updateScore(_score);
				}
			}

		}
		for(auto et : midToDel)
		{
			Enemy * enemyMid = et;
			_enemyLayer->midEnemyBlowup(enemyMid);
		}
	}
	for (auto bt : bulletToDel_Mid)
	{
		Sprite * bullet = bt;
		_bulletLayer->removeBullet(bullet);
	}

	//单发子弹与大敌机碰撞检测
	Vector<Sprite *> bulletToDel_Big;
	for (auto bt : _bulletLayer->_bulletVector)
	{
		Sprite * bullet = bt;
		Vector<Enemy *> bigToDel;
		for (auto et : _enemyLayer->_bigVec)
		{
			Enemy * enemyBig = et;
			if (bullet->getBoundingBox().intersectsRect(enemyBig->Get_BoundingBox()))
			{
				if (enemyBig->getLife() > 1)
				{
					enemyBig->loseLife();
					bulletToDel_Big.pushBack(bullet);
				}
				if (enemyBig->getLife() == 1)
				{
					enemyBig->loseLife();
					bulletToDel_Big.pushBack(bullet);
					bigToDel.pushBack(enemyBig);
					_score += BIG_SCORE;
					_controlLayer->updateScore(_score);
				}
			}

		}
		for(auto et : bigToDel)
		{
			Enemy * enemyBig = et;
			_enemyLayer->bigEnemyBlowup(enemyBig);
		}
	}
	for (auto bt : bulletToDel_Big)
	{
		Sprite * bullet = bt;
		_bulletLayer->removeBullet(bullet);
	}

	//多发子弹与小敌机碰撞检测
	Vector<Sprite *> M_bulletToDel_Small;
	for (auto bt : _multiBulletsLayer->_multiBulletsVector)
	{
		Sprite * M_bullet = bt;
		Vector<Enemy *> M_smallToDel;
		for (auto et : _enemyLayer->_smallVec)
		{
			Enemy * enemy = et;
			if (M_bullet->getBoundingBox().intersectsRect(enemy->Get_BoundingBox()))
			{
				if (enemy->getLife() == 1)
				{
					enemy->loseLife();
					M_bulletToDel_Small.pushBack(M_bullet);
					M_smallToDel.pushBack(enemy);
					_score += SMALL_SCORE;//加上小敌机的分数
					_controlLayer->updateScore(_score);
				}
			}

		}
		for(auto et : M_smallToDel)//注意for循环的位置，要与创建时的语句在同一层
		{
			Enemy * enemy = et;
			_enemyLayer->smallEnemyBlowup(enemy);//敌机爆炸（删除）
		}
	}
	for (auto bt : M_bulletToDel_Small)//注意for循环的位置，要与创建时的语句在同一层
	{
		Sprite * bullet = bt;
		_multiBulletsLayer->removeMultiBullets(bullet);//删除子弹
	}

	//多发子弹与中敌机碰撞检测
	Vector<Sprite *> M_bulletToDel_Mid;
	for (auto bt : _multiBulletsLayer->_multiBulletsVector)
	{
		Sprite * M_bullet = bt;
		Vector<Enemy *> M_midToDel;
		for (auto et : _enemyLayer->_midVec)
		{
			Enemy * enemy = et;
			if(M_bullet->getBoundingBox().intersectsRect(enemy->Get_BoundingBox()))
			{
				if (enemy->getLife() > 0)
				{
					enemy->loseLife();
					M_bulletToDel_Mid.pushBack(M_bullet);
				}
				if (enemy->getLife() == 1)
				{
					enemy->loseLife();
					M_bulletToDel_Mid.pushBack(M_bullet);
					M_midToDel.pushBack(enemy);
					_score += MID_SCORE;
					_controlLayer->updateScore(_score);
				}
			}
		}
		for (auto et : M_midToDel)
		{
			Enemy * enemy = et;
			_enemyLayer->midEnemyBlowup(enemy);
		}
	}
	for (auto bt : M_bulletToDel_Mid)
	{
		Sprite * bullet = bt;
		_multiBulletsLayer->removeMultiBullets(bullet);
	}

	//多发子弹与大敌机碰撞检测
	Vector<Sprite *> M_bulletToDel_Big;
	for (auto bt : _multiBulletsLayer->_multiBulletsVector)
	{
		Sprite * M_bullet = bt;
		Vector<Enemy *> M_bigToDel;
		for (auto et : _enemyLayer->_bigVec)
		{
			Enemy * enemy = et;
			if(M_bullet->getBoundingBox().intersectsRect(enemy->Get_BoundingBox()))
			{
				if (enemy->getLife() > 0)
				{
					enemy->loseLife();
					M_bulletToDel_Big.pushBack(M_bullet);
				}
				if (enemy->getLife() == 1)
				{
					enemy->loseLife();
					M_bulletToDel_Big.pushBack(M_bullet);
					M_bigToDel.pushBack(enemy);
					_score += BIG_SCORE;
					_controlLayer->updateScore(_score);
				}
			}
		}
		for (auto et : M_bigToDel)
		{
			Enemy * enemy = et;
			_enemyLayer->bigEnemyBlowup(enemy);
		}
	}
	for (auto bt : M_bulletToDel_Big)
	{
		Sprite * bullet = bt;
		_multiBulletsLayer->removeMultiBullets(bullet);
	}

	//玩家飞机与小敌机碰撞检测
	Rect myPlane = MyPlane::getInstance()->getBoundingBox();
	for (auto et : _enemyLayer->_smallVec)
	{
		Enemy * enemy = et;
		if (enemy->getLife() > 0)
		{
			if (myPlane.intersectsRect(enemy->Get_BoundingBox()))
			{
				_bulletLayer->stopShoot();
				_multiBulletsLayer->stopShoot();
				this->unscheduleAllSelectors();//停止所有定时器
				MyPlane::getInstance()->PlaneBlowup(_score);//飞机爆炸，传递分数
				//MyPlane::getInstance()->removePlane();
				_enemyLayer->smallEnemyBlowup(enemy);
				/*auto pScene = LayerGameOver::scene(_score);
				Director::getInstance()->replaceScene(pScene);*/
			}
		}
	}

	//玩家飞机与中敌机碰撞检测
	for (auto et : _enemyLayer->_midVec)
	{
		Enemy * enemy = et;
		if (enemy->getLife() > 0)
		{
			if (myPlane.intersectsRect(enemy->Get_BoundingBox()))
			{
				_bulletLayer->stopShoot();
				_multiBulletsLayer->stopShoot();
				this->unscheduleAllSelectors();//停止所有定时器
				MyPlane::getInstance()->PlaneBlowup(_score);//飞机爆炸，传递分数
				//	MyPlane::getInstance()->removePlane();
				//_enemyLayer->midEnemyBlowup(enemy);
				/*auto pScene = LayerGameOver::scene(_score);
				Director::getInstance()->replaceScene(pScene);*/
			}
		}
	}
	//玩家飞机与大敌机碰撞检测
	for (auto et : _enemyLayer->_bigVec)
	{
		Enemy * enemy = et;
		if (enemy->getLife() > 0)
		{
			if (myPlane.intersectsRect(enemy->Get_BoundingBox()))
			{
				_bulletLayer->stopShoot();
				_multiBulletsLayer->stopShoot();
				this->unscheduleAllSelectors();//停止所有定时器
				MyPlane::getInstance()->PlaneBlowup(_score);//飞机爆炸，传递分数
				//	MyPlane::getInstance()->removePlane();
				//_enemyLayer->bigEnemyBlowup(enemy);
				/*auto pScene = LayerGameOver::scene(_score);
				Director::getInstance()->replaceScene(pScene);*/
			}
		}
	}

	//玩家飞机与炸弹道具碰撞检测（在帧循环里面检测的）
	Vector<Sprite *> boomToDel;
	for (auto food : _foodLayer->_bigBoomVec)
	{
		Sprite * bigBoom = food;
		if (bigBoom->getBoundingBox().intersectsRect(MyPlane::getInstance()->getBoundingBox()))
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/get_bomb.wav");
			//_foodLayer->removeBigBoom(bigBoom);
			boomToDel.pushBack(bigBoom);
			_bigBoomCount++;
			this->updateBigBoomCount(_bigBoomCount);
		}
	}
	for (auto bo : boomToDel)
	{
		_foodLayer->removeBigBoom(bo);
	}

	//玩家飞机与多发子弹道具碰撞检测
	Vector<Sprite *> multiToDel;
	for (auto mb : _foodLayer->_multiBulletsVec)
	{
		Sprite * multiBullet = mb;
		if (multiBullet->getBoundingBox().intersectsRect(MyPlane::getInstance()->getBoundingBox()))
		{
			SimpleAudioEngine::getInstance()->playEffect("sound/get_double_laser.wav");
			//_foodLayer->removeMultiBullets(multiBullet);
			multiToDel.pushBack(multiBullet);
			_bulletLayer->stopShoot();
			_multiBulletsLayer->startShoot();
			_score += 100;
			_controlLayer->updateScore(_score);
		}
	}
	for (auto mb : multiToDel)
	{
		_foodLayer->removeMultiBullets(mb);
	}
}

void LayerGameMain::addControlLayer()
{
	_controlLayer = LayerControl::create();
	//要将控制层放在最前面，Z轴坐标(100)比暂停按钮的Z轴坐标(101)
	//小一点使得在点击暂停按钮的时候，其他对象都不能被选中
	this->addChild(_controlLayer,100);
}

void LayerGameMain::addFoodLayer()
{
	_foodLayer = LayerFood::create();
	this->addChild(_foodLayer);
}

void LayerGameMain::updateBigBoomCount(int bigBoomCount)
{
	String strBoomCount;
	Sprite * norBoom = Sprite::createWithSpriteFrameName("bomb.png");
	Sprite * selBoom = Sprite::createWithSpriteFrameName("bomb.png");
	if (bigBoomCount < 0)
	{
		return;
	}
	else if (bigBoomCount == 0)
	{
		if (this->getChildByTag(TAG_BIGBOOM))
		{
			this->removeChildByTag(TAG_BIGBOOM,true);
		}
		if (this->getChildByTag(TAG_BIGBOOMCOUNT))
		{
			this->removeChildByTag(TAG_BIGBOOMCOUNT,true);
		}
	}
	else if (bigBoomCount == 1)
	{
		if ( !(this->getChildByTag(TAG_BIGBOOM)) )
		{
			MenuItemSprite * boomItem = MenuItemSprite::create(norBoom,
															   selBoom,
															   CC_CALLBACK_1(LayerGameMain::boomMenuCallback,this));
			boomItem->setPosition(norBoom->getContentSize().width/2,norBoom->getContentSize().height/2);
			Menu * boomMenu = Menu::create(boomItem,nullptr);
			boomMenu->setPosition(Point::ZERO);
			this->addChild(boomMenu,0,TAG_BIGBOOM);
		}
		if ( !(this->getChildByTag(TAG_BIGBOOMCOUNT)) )
		{
			strBoomCount.initWithFormat("X %d",bigBoomCount);
			LabelBMFont * labelBoomCount = LabelBMFont::create(strBoomCount.getCString(),"font/font.fnt");
			labelBoomCount->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
			labelBoomCount->setPosition(Point(norBoom->getContentSize().width,norBoom->getContentSize().height - 30));
			this->addChild(labelBoomCount,0,TAG_BIGBOOMCOUNT);
		}
	}
	else if (bigBoomCount > 1 && bigBoomCount < MAX_BIGBOOM_COUNT)
	{
		strBoomCount.initWithFormat("X %d",bigBoomCount);
		LabelBMFont * labelCount = (LabelBMFont *)this->getChildByTag(TAG_BIGBOOMCOUNT);
		labelCount->setString(strBoomCount.getCString());
	}
}

void LayerGameMain::boomMenuCallback(cocos2d::Ref * ref)
{
	_bigBoomCount--;
	String strBoomCount;
	strBoomCount.initWithFormat("X %d",_bigBoomCount);
	LabelBMFont * labelCount = (LabelBMFont *)this->getChildByTag(TAG_BIGBOOMCOUNT);
	labelCount->setString(strBoomCount.getCString());

	_score += _enemyLayer->_smallVec.size()*SMALL_SCORE;
	_score += _enemyLayer->_midVec.size()*MID_SCORE;
	_score += _enemyLayer->_bigVec.size()*BIG_SCORE;
	_controlLayer->updateScore(_score);
	_enemyLayer->removeAllEnemy();
	if (_bigBoomCount == 0)
	{
		this->removeChildByTag(TAG_BIGBOOM);
		this->removeChildByTag(TAG_BIGBOOMCOUNT);
	}
}
