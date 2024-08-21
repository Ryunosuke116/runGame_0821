#include"flagSwitch.h"

bool flagSwitch(bool& flg)
{
    if (!flg)
    {
        flg = true;
        return flg;
    }
    else if (flg)
    {
        flg = false;
        return flg;
    }
    
}