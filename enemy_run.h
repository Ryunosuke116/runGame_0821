#pragma once
#include"DxLib.h"
#include"collision_run.h"

enum situation
{
	RUN, DEATH, HIT
};

class Enemy
{
public:
	void enemyInit(sHitRect& enemyHit);

	void Update(float deltaTime,sHitRect& enemyHit);
	void enemyDraw(sHitRect& enemyHit);
	bool scoreFlag = false;
	bool touchFlg = false;			//ìGÇ…êGÇÍÇΩÇ©
	bool backFlg = false;
	int motionFlg = false;
	float moveSpeed = 0;

private:
	VECTOR pos;
};

