#include"collision_run.h"
#include "math.h"
#pragma warning(disable:4996)

void sHitRect::initRect(sHitRect& dstRect, float w, float h)
{
	dstRect.w = w;
	dstRect.h = h;

	dstRect.worldLX = 0.0f;
	dstRect.worldRX = w;
	dstRect.worldLY = 0.0f;
	dstRect.worldRY = h;
}

void sHitRect::updateWorldRect(sHitRect& dstRect, float x, float y)
{
	dstRect.worldLX = x;
	dstRect.worldRX = x + dstRect.w;
	dstRect.worldLY = y;
	dstRect.worldRY = y + dstRect.h;
}

void sHitRect::drawRect(const sHitRect& rect)
{
	DrawBox(static_cast<int>(rect.worldLX),
		static_cast<int>(rect.worldLY),
		static_cast<int>(rect.worldRX - 1),
		static_cast<int>(rect.worldRY - 1),
		GetColor(255, 0, 0), FALSE);
}

bool sHitRect::isHitRect(const sHitRect& rect1, const sHitRect& rect2)
{
	// “ñ‚Â‚Ì‹éŒ`‚Ì“–‚½‚è”»’è‚ÌŒöŽ®‚æ‚è
	return (rect1.worldLX < rect2.worldRX &&
		rect2.worldLX < rect1.worldRX &&
		rect1.worldLY < rect2.worldRY &&
		rect2.worldLY < rect1.worldRY);
}