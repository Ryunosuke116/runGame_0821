#include"modelMotion.h"

int AttachIndex;
float TotalTime, PlayTime;

void model3D::modelInit()
{
	Handle = MV1LoadModel("material/Knight.mv1");
	pos = VGet(0, -30, 0);
	rotate = VGet(0, 0, 0);
	posMax = -25;
	AttachIndex = 0;
	TotalTime = 0;
	PlayTime = 0;
	flg = false;
	motionFlg = false;

	// ３Dモデルのポジション設定
	MV1SetPosition(Handle, pos);

	MV1SetScale(Handle, VGet(10.0f, 10.0f, 10.0f));

	// ３Ｄモデルの０番目のアニメーションをアタッチする
	AttachIndex = MV1AttachAnim(Handle, 21, -1, FALSE);

	// アタッチしたアニメーションの総再生時間を取得する
	TotalTime = MV1GetAttachAnimTotalTime(Handle, AttachIndex);
	TotalTime -= 10;
	// 再生時間の初期化
	PlayTime = 0;

}

void model3D::modelUpdate()
{
	if (flg)
	{
		//徐々に顔を出させる
		if (pos.y < posMax)
		{
			pos.y += 0.1f;

			if (pos.x <= 0)
			{
				pos.x += 0.3f;
			}
			else
			{
				pos.x -= 0.3f;
			}
		}
		else
		{
			pos.y = posMax;
			posMax += 5;
			flg = false;
		}
		
	}

	//ポジション再設定
	MV1SetPosition(Handle, pos);

	if (!motionFlg)
	{
		PlayTime = 0;
	}
	if (motionFlg)
	{
		// 再生時間を進める
		PlayTime += 1.0f;

		// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
		if (PlayTime >= TotalTime)
		{
			PlayTime = TotalTime;
		}
	}


	// 再生時間をセットする
	MV1SetAttachAnimTime(Handle, AttachIndex, PlayTime);
}
void model3D::medolDraw()
{
	//3d描画
	MV1DrawModel(Handle);
}