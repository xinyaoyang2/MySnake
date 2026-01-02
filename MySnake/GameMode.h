#pragma once
#include "common.h"
#include "Map.h"
#include "Snake.h"
#include "Food.h"
#include "Button.h"

class GameMode
{
protected:
	Map gameMap;				//地图
	Snake *snake;				//指向蛇对象的指针
	Food food;					//食物
	bool gameOver;				//游戏结束标志位
	bool gamePaused;			//游戏暂停标志位
	int steps;					//玩家操作步数
	int scores;					//玩家得分
	clock_t startTime;			//游戏开始时间
	clock_t pauseTime;			//游戏暂停时间
	IMAGE pauseScreen;          //保存暂停时的屏幕内容
	std::list<GameRecord> gameRecords; // 存储进入当前模式下的所有游戏记录


	virtual void handleInput();			//处理键盘输入
	virtual void pageInit()=0;			//游戏画面初始化
	virtual bool dateUpdate()=0;		//数据面板更新

	virtual bool Pause()=0;				//游戏暂停
	virtual void Start()=0;				//游戏开始---初始化游戏数据，初始化画面
	virtual void Run()=0;				//游戏运行---游戏的主体逻辑
	virtual bool Over()=0;				//游戏结束--记录游戏数据，然后清除缓存(继续游戏返回1，否则返回0)
public:
	GameMode();
	virtual void GameLoop()=0;			//游戏循环
	std::list<GameRecord> getGameRecords(); // 获取当前模式下的所有游戏记录
	virtual ~GameMode();
};

