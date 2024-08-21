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

	// �RD���f���̃|�W�V�����ݒ�
	MV1SetPosition(Handle, pos);

	MV1SetScale(Handle, VGet(10.0f, 10.0f, 10.0f));

	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	AttachIndex = MV1AttachAnim(Handle, 21, -1, FALSE);

	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	TotalTime = MV1GetAttachAnimTotalTime(Handle, AttachIndex);
	TotalTime -= 10;
	// �Đ����Ԃ̏�����
	PlayTime = 0;

}

void model3D::modelUpdate()
{
	if (flg)
	{
		//���X�Ɋ���o������
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

	//�|�W�V�����Đݒ�
	MV1SetPosition(Handle, pos);

	if (!motionFlg)
	{
		PlayTime = 0;
	}
	if (motionFlg)
	{
		// �Đ����Ԃ�i�߂�
		PlayTime += 1.0f;

		// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
		if (PlayTime >= TotalTime)
		{
			PlayTime = TotalTime;
		}
	}


	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(Handle, AttachIndex, PlayTime);
}
void model3D::medolDraw()
{
	//3d�`��
	MV1DrawModel(Handle);
}