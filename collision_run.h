#pragma once
#include"DxLib.h"

/// <summary>
/// 当たり判定矩形
/// </summary>
class sHitRect
{
private:
public:
	float w = 0;		// 当たり矩形幅ピクセル数
	float h = 0;        // 当たり矩形高さピクセル数
	float worldLX = 0;  // 左上X座標
	float worldLY = 0;  // 左上Y座標
	float worldRX = 0;  // 右下X座標
	float worldRY = 0;  // 右下Y座標

	bool flg = false;

	void initRect(sHitRect& dstRect, float w, float h);


	void updateWorldRect(sHitRect& dstRect, float x, float y);

	void drawRect(const sHitRect& rect);


	bool isHitRect(const sHitRect& rect1, const sHitRect& rect2);

};

