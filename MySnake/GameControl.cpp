#include "GameControl.h"
#include <conio.h> 

#define max(a, b) ((a) > (b) ? (a) : (b))

void GameControl::BGM1()
{
    mciSendString(L"open res/Sound/bgm1.mp3 alias BGM1", 0, 0, 0);
    mciSendString(L"play BGM1 repeat", 0, 0, 0);
    mciSendString(L"stop BGM2 ", 0, 0, 0);
    mciSendString(L"setaudio BGM1 volume to 50", 0, 0, 0);
}

void GameControl::BGM2()
{
    mciSendString(L"open res/Sound/bgm2.mp3 alias BGM2", 0, 0, 0);
    mciSendString(L"play BGM2 repeat", 0, 0, 0);
    mciSendString(L"stop BGM1 ", 0, 0, 0);
    mciSendString(L"setaudio BGM2 volume to 50", 0, 0, 0);
}

void GameControl::LoadingPage()
{
    //打开背景音乐
    BGM1();

    // 加载背景图
    IMAGE bg;
    loadimage(&bg, L"jpg", L"background1", 640, 500,1);

    // 加载小蛇图
    IMAGE snake;
    loadimage(&snake, L"PNG", L"apple", 30, 30, 1);
    //loadimage(&snake, _T("res/Graphics/apple.png"), 30, 30,1);

    // 显示背景图
    putimage(0, 0, &bg);

    // 进度条相关变量
    int progressBarWidth = 400;
    int progressBarHeight = 10;
    int progressBarX = 120; // 使进度条居中
    int progressBarY = 450;

    // 小蛇相关变量
    int snakeX = progressBarX;
    int snakeY = progressBarY - 30;

    // 显示加载进度
    for (int i = 0; i <= 100; ++i) {
        // 清除之前的小蛇图像
        putimage(0, 0, &bg);

        // 重新绘制进度条背景
        setfillcolor(LIGHTGRAY);
        solidrectangle(progressBarX, progressBarY, progressBarX + progressBarWidth, progressBarY + progressBarHeight);

        // 绘制当前进度
        setfillcolor(GREEN);
        solidrectangle(progressBarX, progressBarY, progressBarX + (progressBarWidth * i / 100), progressBarY + progressBarHeight);

        // 绘制小蛇
        drawAlpha(&snake, snakeX + (progressBarWidth * i / 100), snakeY);

        // 刷新屏幕
        FlushBatchDraw();

        // 模拟加载时间
        Sleep(50);
    }
}

void GameControl::MenuPage()
{
    //加载背景
    IMAGE bkg;
    loadimage(&bkg, L"jpg", _T("background11"), 640, 500, 1);
    putimage(0, 0, &bkg);
    
    //页面元素
    Button start_btn(240, 180, 400, 220, 15, _T("开始游戏"), GREEN, WHITE);
    Button record_btn(240, 240, 400, 280, 15, _T("游戏记录"), GREEN, WHITE);
    Button help_btn(240, 300, 400, 340, 15, _T("游戏帮助"), GREEN, WHITE);
    Button return_btn(240, 360, 400, 400, 15, _T("退出游戏"), GREEN, WHITE);

    //消息循环
    ExMessage msg;
    while (true)
    {
        if (peekmessage(&msg, EX_MOUSE, true))
        {
            BeginBatchDraw();
            if (start_btn.handleEvent(msg)) {
                currentPage = Page::GAME_MODE_SELECT;
                break;//退出消息循环
            }
            if (record_btn.handleEvent(msg))//游戏记录
            {
                currentPage = Page::RECORDS;
                break;
            }
            if (help_btn.handleEvent(msg))//游戏帮助
            {
                currentPage = Page::HELP;
                break;
            }
            if (return_btn.handleEvent(msg))//退出游戏
            {
                currentPage = Page::RETURN;
                break;
            }
            EndBatchDraw();
        }
    }
}

void GameControl::ModeSelectPage()
{
    //加载背景
    IMAGE bkg;
    loadimage(&bkg,L"jpg", _T("background2"), 640, 500, 1);
    putimage(0, 0, &bkg);
    loadimage(&bkg, L"jpg", _T("infiniteMode"), 80, 80,1);
    putimage(100, 150, &bkg);
    loadimage(&bkg, L"jpg", _T("timedMode"), 80, 80, 1);
    putimage(460, 150, &bkg);

    //页面元素
    settextcolor(BLACK);
    settextstyle(30, 0, _T("黑体"));
    // 标题“游戏记录”居中显示
    int titleWidth = textwidth(_T("--模式选择--"));
    int titleX = (640 - titleWidth) / 2;
    outtextxy(titleX, 10, _T("--模式选择--"));

    Button back_btn(10, 10, 50, 30, 10, _T("<--"), RED, WHITE); // 返回按钮
    Button infinite_btn(100, 240, 180, 260, 15, _T("无限模式"), GREEN, WHITE);
    Button timed_btn(460, 240, 540, 260, 15, _T("限时模式"), GREEN, WHITE);


    //消息循环
    ExMessage msg;
    while (true)
    {
        if (peekmessage(&msg, EX_MOUSE, true))
        {
            BeginBatchDraw();
            if (back_btn.handleEvent(msg))
            {
                currentPage = Page::MENU;
                break;
            }
            if (infinite_btn.handleEvent(msg))
            {
                currentGameMode = new InfiniteMode();
                currentPage = Page::GAME;
                break;
            }
            if (timed_btn.handleEvent(msg))
            {
                //InputBox()
                currentGameMode = new TimedMode(60);
                currentPage = Page::GAME;
                break;
            }
            EndBatchDraw();
        }
    }
}

void GameControl::GamePage()
{
    if (currentGameMode == nullptr)
        return;

    // 清空键盘消息
    while (_kbhit()) {
        _getch(); // 读取并丢弃按键
    }
    BGM2();
    currentGameMode->GameLoop();
    addGameRecords(currentGameMode->getGameRecords());
    writeRecordsToCSV(FILENAME, records);//写入文件保存
    delete currentGameMode;
    currentGameMode = nullptr;
    currentPage = Page::MENU;
    BGM1();
}

void GameControl::RecordPage()
{
    // 设置背景
    setbkcolor(RGB(128, 96, 26));
    cleardevice();
    setfillcolor(WHITE);
    solidrectangle(10, 50, 630, 470);

    // 页面元素
    Button back_btn(10, 10, 50, 30, 10, _T("<--"), RED, WHITE); // 返回按钮

    setbkcolor(WHITE);
    settextcolor(BLACK);
    settextstyle(30, 0, _T("黑体"));
    // 标题“游戏记录”居中显示
    int titleWidth = textwidth(_T("游戏记录"));
    int titleX = (640 - titleWidth) / 2;
    outtextxy(titleX, 10, _T("游戏记录"));

    int scrollOffset = 0;
    // 消息循环
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_MOUSE | EX_KEY, true)) 
        {
            BeginBatchDraw();

            // 清除屏幕重新绘制
            solidrectangle(10, 50, 630, 470);

            // 绘制记录
            settextcolor(BLACK);
            settextstyle(20, 0, _T("Consolas"));

            // 打印标题行
            TCHAR buffer[200];
            _stprintf(buffer, _T("%-8s%-8s%-8s%-8s%-12s"),
                _T("游戏模式"), _T("游戏得分"), _T("游戏步数"), _T("运行时间(秒)"), _T("游戏时间"));
            outtextxy(20, 60, buffer);
            int y = 90 - scrollOffset;

            for (const auto& record : records) {
                if (y >= 90 && y < 470) { // 只绘制可见区域内的记录
                    // 将 time_t 转换为 tm 结构体
                    struct tm* localTime = localtime(&record.recordTime);

                    // 格式化游戏时间
                    TCHAR timeBuffer[100];
                    _tcsftime(timeBuffer, sizeof(timeBuffer), _T("%Y-%m-%d %H:%M"), localTime);

                    // 格式化字符串，确保每个字段的宽度固定
                    _stprintf(buffer, _T("%-8s%-12d%-12d%-12d%-12s"),
                        record.mode.c_str(), record.score, record.steps, record.time, timeBuffer);
                    outtextxy(20, y, buffer);
                }
                y += 30;
            }

            // 按键响应
            if (back_btn.handleEvent(msg)) {
                currentPage = Page::MENU;
                break;
            }

            // 处理鼠标滚轮滚动
            if (msg.message == WM_MOUSEWHEEL) {
                int zDelta = msg.wheel;
                scrollOffset -= zDelta / 120 * 30; // 每次滚动30像素
                if (scrollOffset < 0) scrollOffset = 0; // 防止滚动超出顶部
                if (scrollOffset > max(0, (int(records.size()) * 30 - 420))) {
                    scrollOffset = max(0, (int(records.size()) * 30 - 420)); // 防止滚动超出底部
                }
            }
            EndBatchDraw();
        }
    }
}

void GameControl::HelpPage()
{
    // 设置背景
    setbkcolor(RGB(128, 96, 26));
    cleardevice();
    setfillcolor(WHITE);
    solidrectangle(10, 50, 630, 470);

    // 页面元素
    Button back_btn(10, 10, 50, 30, 10, _T("<--"), RED, WHITE); // 返回按钮

    setbkcolor(WHITE);
    settextcolor(BLACK);
    settextstyle(30, 0, _T("黑体"));
    // 标题“游戏记录”居中显示
    int titleWidth = textwidth(_T("游戏帮助"));
    int titleX = (640 - titleWidth) / 2;
    outtextxy(titleX, 10, _T("游戏帮助"));

    settextcolor(BLUE);
    settextstyle(20, 0, L"宋体");
    outtextxy(230, 150, L"↑ ← ↓ → 控制方向");
    outtextxy(230, 180, L"w  a  s  d  控制方向");
    outtextxy(230, 210, L"速度等级1-25，默认12");
    outtextxy(240, 240, L"c键加速，x键减速");
    outtextxy(240, 270, L"z键恢复原始速度");
    outtextxy(240, 300, L"空格键暂停/继续");

    outtextxy(180, 350, L"请将输入法调至英文输入法状态下");

    ExMessage msg;
    while (true)
    {
        if (peekmessage(&msg, EX_MOUSE, true))
        {
            BeginBatchDraw();
            if (back_btn.handleEvent(msg))
            {
                currentPage = Page::MENU;
                break;
            }
            EndBatchDraw();
        }
    }

}

void GameControl::MainLoop()
{
    LoadingPage();//游戏加载页面
    while (true)
    {
        switch (currentPage)
        {
        case Page::MENU:
            this->MenuPage();
            break;
        case Page::GAME_MODE_SELECT:
            this->ModeSelectPage();
            break;
        case Page::GAME:
            this->GamePage();
            break;
        case Page::RECORDS:
            this->RecordPage();
            break;
        case Page::HELP:
            this->HelpPage();
            break;
        case Page::RETURN://退出游戏
            return;
        default:
            break;
        }
    }
}

// 构造函数
GameControl::GameControl() : currentGameMode(nullptr), currentPage(Page::MENU),records(readRecordsFromCSV(FILENAME)){}

// 析构函数
GameControl::~GameControl() {
    if (currentGameMode) {
        delete currentGameMode;
    }
}

// 添加游戏记录
void GameControl::addGameRecords(const std::list<GameRecord>& newRecords) {
    records.insert(records.end(), newRecords.begin(), newRecords.end());
}
