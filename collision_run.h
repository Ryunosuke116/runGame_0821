#pragma once
#include"DxLib.h"

/// <summary>
/// �����蔻���`
/// </summary>
class sHitRect
{
private:
public:
	float w = 0;		// �������`���s�N�Z����
	float h = 0;        // �������`�����s�N�Z����
	float worldLX = 0;  // ����X���W
	float worldLY = 0;  // ����Y���W
	float worldRX = 0;  // �E��X���W
	float worldRY = 0;  // �E��Y���W

	bool flg = false;

	void initRect(sHitRect& dstRect, float w, float h);


	void updateWorldRect(sHitRect& dstRect, float x, float y);

	void drawRect(const sHitRect& rect);


	bool isHitRect(const sHitRect& rect1, const sHitRect& rect2);

};

