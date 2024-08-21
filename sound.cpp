#include"sound.h"

void playBackBGM(int& sound)
{
	PlaySoundMem(sound, DX_PLAYTYPE_LOOP);
}

void stopBGM(int& sound)
{
	StopSoundMem(sound);
}