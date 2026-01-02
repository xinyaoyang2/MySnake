#include "InfiniteMode.h"

// 构造函数
InfiniteMode::InfiniteMode() : GameMode() {}

void InfiniteMode::pageInit() {
    // 自定义无限模式的页面初始化
    setbkcolor(WHITE);
    cleardevice();
    setfillcolor(RGB(128, 96, 26));
    solidrectangle(0, 0, 500, 500);
    settextstyle(16, 0, _T("宋体"));
    settextcolor(BLACK);

    // 分数显示
    outtextxy(530, 120, _T("分数："));
    IMAGE foodImg;
    loadimage(&foodImg, L"PNG", _T("apple"), 16, 16, 1);
    drawAlpha(&foodImg, 510, 120);

    // 速度显示
    outtextxy(530, 150, _T("速度："));

    // 步数显示
    outtextxy(530, 180, _T("步数："));

    // 时间显示
    outtextxy(530, 210, _T("时间："));

    gameMap.drawALLMap(); // 更新地图

    // 按键提示
    outtextxy(510, 240, _T("[SPACE] : 暂停"));
    outtextxy(510, 270, _T("[X] :     减速"));
    outtextxy(510, 300, _T("[C] :     加速"));
}

bool InfiniteMode::dateUpdate() {
    // 自定义无限模式的数据更新
    settextcolor(0xFF5555); // 亮蓝
    setfillcolor(WHITE);
    settextstyle(16, 0, _T("Consolas"));

    // 分数显示
    TCHAR t[10];
    _stprintf_s(t, _T("%d"), scores);
    solidrectangle(590, 120, 640, 140);
    outtextxy(590, 120, t);

    // 速度显示
    _stprintf_s(t, _T("%d"), snake->getSpeed());
    solidrectangle(590, 150, 640, 170);
    outtextxy(590, 150, t);

    // 步数显示
    _stprintf_s(t, _T("%d"), steps);
    solidrectangle(590, 180, 640, 200);
    outtextxy(590, 180, t);

    // 时间显示
    clock_t currentTime = clock();
    int elapsedTime = (gamePaused ? pauseTime : currentTime) - startTime;
    int seconds = (elapsedTime / CLOCKS_PER_SEC) % 60;
    int minutes = (elapsedTime / CLOCKS_PER_SEC) / 60;
    TCHAR timeStr[20];
    _stprintf(timeStr, _T("%02d:%02d"), minutes, seconds);
    solidrectangle(590, 210, 640, 230);
    outtextxy(590, 210, timeStr);

    return false;
}

bool InfiniteMode::Pause() {
    // 自定义暂停逻辑
    // 保存界面
    getimage(&pauseScreen, 0, 0, 640, 500);
    // 定义提示区域的大小和位置
    int rectWidth = 300;
    int rectHeight = 200;
    int centerX = 640 / 2;
    int centerY = 500 / 2;
    int left = centerX - rectWidth / 2;
    int top = centerY - rectHeight / 2;
    int right = centerX + rectWidth / 2;
    int bottom = centerY + rectHeight / 2;

    // 定义深绿色和紫色
    COLORREF darkGreen = RGB(0, 100, 0);
    COLORREF purple = RGB(128, 0, 128);

    // 绘制提示区域
    setfillcolor(darkGreen);
    solidrectangle(left, top, right, bottom);

    setlinecolor(purple);
    setbkcolor(darkGreen);
    rectangle(left, top, right, bottom);

    settextstyle(30, 0, _T("微软雅黑"));
    settextcolor(BLACK);
    outtextxy(centerX - 120, centerY - 90, _T("！游戏暂停"));

    settextstyle(16, 0, _T("黑体"));
    outtextxy(centerX - 100, centerY - 40, _T("是否继续游戏"));

    Button btn1(centerX - 100, centerY + 40, centerX - 40, centerY + 70, 10, _T("继续游戏"), WHITE, BLACK);
    Button btn2(centerX + 40, centerY + 40, centerX + 100, centerY + 70, 10, _T("结束游戏"), WHITE, BLACK);
    ExMessage msg;
    flushmessage();
    while (true) {
        BeginBatchDraw();
        if (peekmessage(&msg, EX_MOUSE, true)) {
            if (btn1.handleEvent(msg)) return true;
            if (btn2.handleEvent(msg)) return false;
        }
        EndBatchDraw();
        Sleep(10);
    }
    return false;
}

void InfiniteMode::Start() {
    // 自定义开始逻辑
    pageInit();
    snake = new Snake(); // 实例化一个蛇类
    gameOver = false;
    gamePaused = false;
    scores = 0;
    steps = 0;
    startTime = clock(); // 计时器开始
    dateUpdate(); // 更新数据面板

    // 清空键盘消息
    while (_kbhit()) {
        _getch(); // 读取并丢弃按键
    }
}

void InfiniteMode::Run() {
    // 自定义运行逻辑
    while (!gameOver) {
        // 食物的生成
        if (food.getState() == false) {
            food.Generate(gameMap, snake->GetSnakeAllNode());
        }
        // 获取按键响应--移动，加速，暂停
        handleInput();
        // 游戏是否暂停
        if (gamePaused == true) {
            pauseTime = clock();
            if (Pause() == false) {
                // 结束游戏
                gameOver = true;
                continue;
            }
            // 清空键盘消息
            while (_kbhit()) {
                _getch(); // 读取并丢弃按键
            }
            gamePaused = false;
            putimage(0, 0, &pauseScreen);
            startTime += clock() - pauseTime;
        }
        // 蛇移动
        snake->Move();
        // 吃食物
        if (snake->ColideFood(food.getx(), food.gety())) {
            snake->EatFood();
            food.setState(false);
            scores++;
        }

        // 碰撞检测
        if (snake->ColideWall(gameMap) || snake->ColideSnake()) {
            // 游戏结束
            gameOver = true;
            continue;
        }
        // 画蛇
        BeginBatchDraw();
        food.drawFood(gameMap);       // 绘制食物
        snake->DrawSnake(gameMap);   // 绘制蛇
        EndBatchDraw();

        // 更新数据面板
        dateUpdate();

        // 速度控制
        int sleep = 30 - snake->getSpeed();
        Sleep(sleep * 10);
    }
}

bool InfiniteMode::Over() {
    // 记录游戏数据
    gameRecords.push_back(GameRecord(_T("无限模式"), scores, steps, (clock() - startTime) / CLOCKS_PER_SEC));
    //注销数据
    delete snake;

    // 定义提示区域的大小和位置
    int rectWidth = 300;
    int rectHeight = 300; // 扩大高度以显示更多信息
    int centerX = 640 / 2;
    int centerY = 500 / 2;
    int left = centerX - rectWidth / 2;
    int top = centerY - rectHeight / 2;
    int right = centerX + rectWidth / 2;
    int bottom = centerY + rectHeight / 2;

    // 定义深绿色和紫色
    COLORREF darkGreen = RGB(0, 100, 0);
    COLORREF purple = RGB(128, 0, 128);

    // 绘制提示区域
    setfillcolor(darkGreen);
    solidrectangle(left, top, right, bottom);

    setlinecolor(purple);
    setbkcolor(darkGreen);
    rectangle(left, top, right, bottom);

    // 计算游戏时间
    clock_t currentTime = clock();
    int elapsedTime = (currentTime - startTime) / CLOCKS_PER_SEC;
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;

    // 绘制 "GAME OVER" 标题
    settextstyle(30, 0, _T("Consolas"));
    settextcolor(WHITE);
    outtextxy(centerX - textwidth(_T("GAME OVER")) / 2, centerY - 120, _T("GAME OVER"));

    // 显示游戏表现情况
    settextstyle(20, 0, _T("黑体"));
    settextcolor(WHITE);
    TCHAR scoreStr[50];
    _stprintf(scoreStr, _T("得分: %d"), scores);
    outtextxy(centerX - textwidth(scoreStr) / 2, centerY - 60, scoreStr);

    TCHAR stepsStr[50];
    _stprintf(stepsStr, _T("步数: %d"), steps);
    outtextxy(centerX - textwidth(stepsStr) / 2, centerY - 30, stepsStr);

    TCHAR timeStr[50];
    _stprintf(timeStr, _T("时间: %02d:%02d"), minutes, seconds);
    outtextxy(centerX - textwidth(timeStr) / 2, centerY, timeStr);

    Button btn1(centerX - 100, centerY + 40, centerX - 40, centerY + 70, 10, _T("再来一局"), WHITE, BLACK);
    Button btn2(centerX + 40, centerY + 40, centerX + 100, centerY + 70, 10, _T("退出游戏"), WHITE, BLACK);
    ExMessage msg;
    flushmessage();
    while (true) {
        BeginBatchDraw();
        if (peekmessage(&msg, EX_MOUSE, true)) {
            if (btn1.handleEvent(msg)) return true;
            if (btn2.handleEvent(msg)) return false;
        }
        EndBatchDraw();
        Sleep(10);
    }
    return false;
}

void InfiniteMode::GameLoop() {
    while (1) {
        Start();
        Run(); // 游戏暂停逻辑包含其中。
        if (!Over())
        {
            break;
        }
    }
    // 退出该游戏模式
   
}
