#include"backGround.h"


	int	modelHandle;
	VECTOR	pos;
	VECTOR Rotate;
	int x;
	float y;
	int ground[3];
	int explanationGraph;
	float cameraX, cameraY, cameraZ;
	bool cameraFlg;

void backInit(model3D& model)
{
	// グラフィックの描画先を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	//奥行0.1～1000までをカメラの描画範囲とする
	SetCameraNearFar(0.1f, 2000.0f);

	//(0,10,-20)の視点から(0,10,0)のターゲットを見る角度にカメラを設置
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -20),
		VGet(0.0f, 0.0f, 0.0f));

	// スカイドームの読み込み
	modelHandle = MV1LoadModel("material/Dome_X501.mv1");

	pos = VGet(0.0f, -20.0f, 0);

	Rotate = VGet(0.0f, 0.0f, 0);

	// スカイドームのポジション設定
 	MV1SetPosition(modelHandle, pos);

	//3Dモデル初期設定
	model.modelInit();

	ground[0] = LoadGraph("material/Background layers/Layer_0010_1.png");
	ground[1] = LoadGraph("material/Layer_0001_8.png");
	ground[2] = LoadGraph("material/Layer_0000_9_big.png");
	explanationGraph = LoadGraph("material/dialog box jump.png");
	x = 0;
	y = 0;
	cameraX = 0;
	cameraY = 0;
	cameraZ = -20;
	cameraFlg = false;
}

void backUpdate(Enemy& enemy)
{
	

	if (CheckHitKey(KEY_INPUT_W))
	{
		cameraZ += 1;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		cameraZ -= 1;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		cameraX += 1;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		cameraX -= 1;
	}

	SetCameraPositionAndTarget_UpVecY(VGet(cameraX, cameraY, cameraZ),
		VGet(0.0f, 0.0f, 0.0f));

	if (enemy.motionFlg != DEATH)
	{
		//y += 0.01f;
		if (y >= 360.0f)
		{
			//y = 0;
		}
	}

	Rotate.y -= 0.001f;

	if (enemy.motionFlg != DEATH)
	{
		x -= 5;
		if (x <= -1920)
		{
			x = 0;
		}

	}
}

void backDraw(Enemy& enemy,model3D& model)
{

	DrawGraph(0, 0, ground[0], TRUE);
	
	//3d描画
	MV1DrawModel(modelHandle);
	model.medolDraw();
	
	DrawGraph(x, 730, ground[1], TRUE);
	DrawGraph(x, 590, ground[2], TRUE);

	DrawGraph(1920 + x, 730, ground[1], TRUE);
	DrawGraph(1920 + x, 590, ground[2], TRUE);

 	DrawGraph(100, 820, explanationGraph, TRUE);

	MV1SetRotationXYZ(modelHandle, Rotate);

	// プレイヤー描画.
}

void backDraw()
{
	DrawGraph(0, 0, ground[0], TRUE);
}