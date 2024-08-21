#pragma once
#include"DxLib.h"
#include"enemy_run.h"
#include"player_run.h"
#include"gameStatus.h"
#include"score.h"
#include"backGround.h"
#include"modelMotion.h"


const int NOTICE_COUNT = 20;

//âÊñ èÛë‘ÇÃñºèÃ
enum Status
{
	TITLE,RULE, STAGE, Stage_2, OVER, CLEAR, RESULT
};

void gameStatusInit();
void title(int& GameStatus, Player& player, sHitRect& playerHit,
	Enemy& enemy, sHitRect& enemyHit, model3D& model);
void stage(int& gameStatus, Enemy& enemy, model3D& model);
void stageDraw();
void rule(int& gameStatus, Player& player, sHitRect& playerHit,
	Enemy& enemy, sHitRect& enemyHit, model3D model);
void ruleDraw();
void gameOver(int& gameStatus);
void clear(int& gameStatus);
void result(int& gameStatus);

void calculationCount(int& timeCount);