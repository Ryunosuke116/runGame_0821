#pragma once
#include"DxLib.h"
#include"collision_run.h"
#include"enemy_run.h"

const int SCORE = 50;
const int PLUS_SCORE = 20;

void scoreUIInit();

void scoreDraw(Enemy& enemy);

void scoreUpdate(sHitRect& playerHit, sHitRect& enemyHit, Enemy& enemy);

void scoreCalculation(int& score_result);

void keep(int scoreKeep[]);

void combo(int& comboStack);