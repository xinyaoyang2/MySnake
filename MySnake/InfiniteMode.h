#pragma once
#include "common.h"
#include "GameMode.h"
#include "Map.h"
#include "Snake.h"
#include "Food.h"
#include "Button.h"

class InfiniteMode:public GameMode
{
protected:
    void pageInit() override;        // 游戏画面初始化
    bool dateUpdate() override;      // 数据面板更新
    bool Pause() override;           // 游戏暂停
    void Start() override;           // 游戏开始
    void Run() override;             // 游戏运行
    bool Over() override;            // 游戏结束

public:
    InfiniteMode();
    void GameLoop() override;        // 游戏循环
};
