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
	// �O���t�B�b�N�̕`���𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	//���s0.1�`1000�܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(0.1f, 2000.0f);

	//(0,10,-20)�̎��_����(0,10,0)�̃^�[�Q�b�g������p�x�ɃJ������ݒu
	SetCameraPositionAndTarget_UpVecY(VGet(0, 0, -20),
		VGet(0.0f, 0.0f, 0.0f));

	// �X�J�C�h�[���̓ǂݍ���
	modelHandle = MV1LoadModel("material/Dome_X501.mv1");

	pos = VGet(0.0f, -20.0f, 0);

	Rotate = VGet(0.0f, 0.0f, 0);

	// �X�J�C�h�[���̃|�W�V�����ݒ�
 	MV1SetPosition(modelHandle, pos);

	//3D���f�������ݒ�
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
	
	//3d�`��
	MV1DrawModel(modelHandle);
	model.medolDraw();
	
	DrawGraph(x, 730, ground[1], TRUE);
	DrawGraph(x, 590, ground[2], TRUE);

	DrawGraph(1920 + x, 730, ground[1], TRUE);
	DrawGraph(1920 + x, 590, ground[2], TRUE);

 	DrawGraph(100, 820, explanationGraph, TRUE);

	MV1SetRotationXYZ(modelHandle, Rotate);

	// �v���C���[�`��.
}

void backDraw()
{
	DrawGraph(0, 0, ground[0], TRUE);
}