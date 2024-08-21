#pragma once
#include"DxLib.h"

class model3D
{
private:
    VECTOR pos;
    VECTOR rotate;
    int	Handle;
    float posMax;
public:
    bool flg;
    bool motionFlg;
    void medolDraw();

    void modelUpdate();


    void modelInit();
};

