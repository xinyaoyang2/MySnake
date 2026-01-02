#include <cmath>
#include "common.h"

// 将游戏记录写入 CSV 文件的通用函数
void writeRecordsToCSV(const std::wstring & filename, const std::list<GameRecord>&records) {
    std::wfstream file;
    file.imbue(std::locale("chs"));
    file.open(filename, std::ios::out);

    if (!file.is_open()) {
        return;
    }
    // 写入表头
    file << _T("模式,得分,步数,时间,记录时间\n");
    
    // 写入每条记录
    for (const auto& record : records) {
        std::wstringstream ss;
        ss << record.recordTime;
        file << record.mode << _T(",")
            << record.score << _T(",")
            << record.steps << _T(",")
            << record.time << _T(",")
            << ss.str() << L"\n";
    }

    file.close();
}

// 从 CSV 文件读取游戏记录的通用函数
std::list<GameRecord> readRecordsFromCSV(const std::wstring& filename) {
    std::wifstream file(filename);
    file.imbue(std::locale("chs"));
    std::list<GameRecord> records;

    if (!file.is_open()) {
        return records;
    }

    std::wstring line;
    std::getline(file, line); // 跳过表头

    while (std::getline(file, line)) {
        std::wstringstream ss(line);
        std::wstring mode, timeStr;
        int score, steps, time;
        std::time_t recordTime;

        std::getline(ss, mode, L',');
        ss >> score;
        ss.ignore(1, L',');
        ss >> steps;
        ss.ignore(1, L',');
        ss >> time;
        ss.ignore(1, L',');
        ss >> recordTime;

        records.emplace_back(mode, score, steps, time, recordTime);
    }
    
    file.close();
    return records;
}

// 载入PNG图并去透明部分
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD* dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD* src = GetImageBuffer(picture); // 获取picture的显存指针
    int picture_width = picture->getwidth(); // 获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); // 获取picture的高度，EASYX自带
    int graphWidth = getwidth();       // 获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     // 获取绘图区的高度，EASYX自带

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++) {
        for (int ix = 0; ix < picture_width; ix++) {
            int srcX = ix + iy * picture_width; // 在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA是透明度
            if (sa == 0) continue; // 如果透明度为0，跳过

            int sr = ((src[srcX] & 0xff0000) >> 16); // 获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   // G
            int sb = src[srcX] & 0xff;              // B

            int dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // 在显存里像素的角标
            if (ix + picture_x < 0 || ix + picture_x >= graphWidth || iy + picture_y < 0 || iy + picture_y >= graphHeight) continue;

            int dr = ((dst[dstX] & 0xff0000) >> 16);
            int dg = ((dst[dstX] & 0xff00) >> 8);
            int db = dst[dstX] & 0xff;

            dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  // 公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         // αp=sa/255 , FP=sg , BP=dg
                | (sb * sa / 255 + db * (255 - sa) / 255);              // αp=sa/255 , FP=sb , BP=db
        }
    }
}