#pragma once
#include"DxLib.h"
#include"enemy_run.h"
#include"collision_run.h"

const float width = 50.0f;
const int correX = 130;
const int correY = 90;

class Player
{
public:

	Player();
	~Player();

	void playerInit(sHitRect& playerHit, Enemy& enemy);

	void Update(float deltaTime,
		Enemy& enemy, sHitRect& playerHit, sHitRect& enemyHit, int& gameStatus);

	void playerDraw( sHitRect& playerHit, Enemy& enemy, int& gameStatus);

private:
    VECTOR pos;

	bool jumpFlag = false;
	bool onGround = true;					 // ���n���Ă��邩�H
	bool HitonGround = true;				 // ���n���Ă��邩�H
	bool isJumpButtonDown;					// �W�����v�{�^�����������ςȂ����H
	bool touchFlg = false;

    float moveSpeed;
	float currentJumpSpeed = 0;	 // ���݂̃W�����v�̃X�s�[�h
	float JumpPower = 18.5f;
	float Gravity = 1.0f;

	int HP = 0;
	int W = 0;									
	int H = 0;
	int score = 0;
};
