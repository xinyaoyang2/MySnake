#pragma once
#pragma comment (lib, "winmm.lib")
#include <Windows.h>
#include <mmsystem.h>
#include <graphics.h>
#include <tchar.h>
#include <conio.h>
#include <list>
#include <iostream> 
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>

//保存游戏记录文件名
#define FILENAME _T("res/GameRecord/gameRecord.csv")

//游戏记录
struct GameRecord {
    std::basic_string<TCHAR> mode;       // 游戏模式（无限模式/限时模式）
    int score;              // 得分
    int steps;              // 步数
    int time;               // 游戏时间（秒）
    std::time_t recordTime;  // 记录生成时间

    GameRecord(std::basic_string<TCHAR> m, int s, int st, int t)
        : mode(m), score(s), steps(st), time(t), recordTime(std::time(nullptr)) {}
    GameRecord(std::wstring m, int s, int st, int t, std::time_t rt)
        : mode(m), score(s), steps(st), time(t), recordTime(rt) {}
};

//把游戏记录写入文件
void writeRecordsToCSV(const std::wstring& filename, const std::list<GameRecord>& records);

//把游戏记录从文件中读取出来
std::list<GameRecord> readRecordsFromCSV(const std::wstring& filename);

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);
