#include "GameControl.h"
#include "resource.h"


int main()
{
    initgraph(640, 500);
    mciSendString(L"open res/Sound/bgm1.mp3 alias BGM1", 0, 0, 0);
    mciSendString(L"play BGM1 repeat", 0, 0, 0);
    //mciSendString(L"stop BGM2 ", 0, 0, 0);
    mciSendString(L"setaudio BGM1 volume to 50", 0, 0, 0);
    GameControl gameControl;
    gameControl.MainLoop();
    closegraph();
    return 0;
}

