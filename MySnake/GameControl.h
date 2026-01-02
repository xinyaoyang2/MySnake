#pragma once

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>

#include "common.h"
#include "Button.h"
#include "Snake.h"
#include "Map.h"
#include "Food.h"
#include "GameMode.h"
#include "InfiniteMode.h"
#include "TimedMode.h"

//界面的枚举体
enum class Page {
    MENU,                   //菜单界面
    GAME_MODE_SELECT,       //游戏模式选择界面
    GAME,                   //开始游戏--正式的游戏界面（游戏模式选择完毕后进入）
    RECORDS,                //游戏记录
    HELP,                    //帮助界面
    RETURN                  //退出游戏
};

class GameControl {
private:
    GameMode* currentGameMode;        // 当前游戏模式指针
    std::list<GameRecord> records;    // 游戏记录
    Page currentPage;                 // 当前页面

    void addGameRecords(const std::list<GameRecord>& newRecords); // 添加游戏记录

    //背景音乐播放
    void BGM1();
    void BGM2();

    //不同的界面
    void LoadingPage();         //游戏开始的加载界面
    void MenuPage();            //菜单页面--主页面
    void ModeSelectPage();      //模式选择界面
    void GamePage();            //游戏运行界面
    void RecordPage();          //游戏记录界面
    void HelpPage();            //游戏帮助界面

public:
    GameControl();
    void MainLoop();            //整个游戏的调度循环
    ~GameControl();

};