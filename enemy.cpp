#include"enemy_run.h"
#include"collision_run.h"
#include"score.h"

const int	ImageSizeY = 150;			//�L�����̏c�̐��l
const int	ImageSizeX = 150;			//�L�����̉��̐��l
const int enemyWidth = 80;
const int animationPatternNum = 8;	//�A�j���[�V�����̃p�^�[��
const int TransparentAnimationPatternNum = 4;
const int animationTypeNum = 1;		//�A�j���[�V�����̎��
const float animationFPS = 6.0f;	//�b��6�̃A�j���[�V����

int enemyColor = GetColor(255, 255, 0);

int enemyGraph[animationPatternNum * animationTypeNum];
int enemyAnimationNowType;					//���݂̃A�j�����
int enemyAnimationNowPattern;					//���݂̃A�j���p�^�[��
int enemyAnimationNowIndex;					//�A�j���[�V�����摜�z��̓Y����0�`11�܂ł̐��l
float enemyAnimationTimer;				//�A�j���[�V�����^�C�}�[

int enemyTransparentGraph[TransparentAnimationPatternNum * animationTypeNum];
int TransparentAnimationNowType;			//���݂̃A�j�����
int TransparentAnimationNowPattern;			//���݂̃A�j���p�^�[��
int TransparentAnimationNowIndex;			//�A�j���[�V�����摜�z��̓Y����0�`11�܂ł̐��l
float TransparentAnimationTimer;			//�A�j���[�V�����^�C�}�[


int enemyCollision;
int comboStack;

bool transparentFlg = false;		//���[�V�����ύX
bool stopFlg;

VECTOR inputVec;
VECTOR velocity = VGet(0.0f, 0.0f, 0.0f);

void Enemy::enemyInit(sHitRect& enemyHit)
{
	pos = VGet(1920.0f, 680.0f, 0.0f);
	inputVec = VGet(0.0f, 0.0f, 0.0f);
	moveSpeed = 10.0f;
	scoreFlag = false;
	touchFlg = false;
	transparentFlg = false;
	stopFlg = false;
	backFlg = false;
	comboStack = 0;

	enemyAnimationNowType = 0;
	enemyAnimationNowPattern = 0;
	enemyAnimationNowIndex = 0;
	enemyAnimationTimer = 0;

	TransparentAnimationNowType = 0;
	TransparentAnimationNowPattern = 3;
	TransparentAnimationNowIndex = 0;
	TransparentAnimationTimer = 0;

	LoadDivGraph("material/Monsters_Creatures_Fantasy/Flying eye/Flight.png",
		animationPatternNum * animationTypeNum,
		animationPatternNum,
		animationTypeNum,
		ImageSizeX, ImageSizeY,
		enemyGraph);

	LoadDivGraph("material/Enemy_Death.png",
		TransparentAnimationPatternNum * animationTypeNum,
		TransparentAnimationPatternNum,
		animationTypeNum,
		ImageSizeX, ImageSizeY,
		enemyTransparentGraph);

	enemyHit.initRect(enemyHit, enemyWidth, enemyWidth);
}

void Enemy::Update(float deltaTime,sHitRect& enemyHit)
{
	inputVec = VGet(-1.0f, 0.0f, 0.0f);
	
	//���K��
	if (inputVec.x != 0)
	{
		inputVec = VNorm(inputVec);
	}

	//�ړ��ʌv�Z
	velocity = VScale(inputVec, moveSpeed);

	if (motionFlg != DEATH)
	{
		pos = VAdd(pos, velocity);
	}

	if (pos.x <= 0 && !backFlg)
	{
		pos.x = 1920.0f;
		scoreFlag = false;
		touchFlg = false;
		backFlg = true;
		stopFlg = false;
	}

	combo(comboStack);
	enemyHit.updateWorldRect(enemyHit, pos.x, pos.y);

	//10�R���{�ȏ�̂Ƃ��A�����ɂȂ郂�[�V����
	if (comboStack >= 10 && backFlg)
	{
		//1400�̒n�_�œ����ɂȂ�
		if (pos.x <= 1400 && pos.x >= 1300)
		{
			if (!stopFlg)
			{
				pos.x = 1400;
			}

			transparentFlg = true;

			// �Ȃ����玟�̃R�}�ɐi�߂�
			TransparentAnimationTimer += deltaTime;
			if (TransparentAnimationTimer > 1.0f / animationFPS+1.0f)
			{
				// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
				TransparentAnimationTimer = 0.0f;
				TransparentAnimationNowPattern--;
				if (TransparentAnimationNowPattern <= 0)
				{
					stopFlg = true;
					TransparentAnimationNowPattern = 0;
				}
			}

		}

	// �ŏI�I�ȓY�������v�Z 
	 // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
	TransparentAnimationNowIndex = TransparentAnimationNowPattern +
		TransparentAnimationNowType * TransparentAnimationPatternNum;

	}
	if (comboStack >= 10)
	{
		if (pos.x <= 720 && backFlg)
		{
			if (pos.x <= 720)
			{
				pos.x = 720;
			}

			stopFlg = false;

			// �Ȃ����玟�̃R�}�ɐi�߂�
			TransparentAnimationTimer += deltaTime;
			if (TransparentAnimationTimer > 1.0f / animationFPS + 1.0f)
			{
				// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
				TransparentAnimationTimer = 0.0f;
				TransparentAnimationNowPattern++;
				if (TransparentAnimationNowPattern >= 3)
				{
					backFlg = false;
					stopFlg = true;
					TransparentAnimationNowPattern = 3;
				}
			}

			// �ŏI�I�ȓY�������v�Z 
	 // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
			TransparentAnimationNowIndex = TransparentAnimationNowPattern +
				TransparentAnimationNowType * TransparentAnimationPatternNum;
		}
	}

	//700�̒n�_�Ō�����悤�ɂȂ�
	if (pos.x <= 700)
	{
		transparentFlg = false;
		backFlg = false;
	}


	// �Ȃ����玟�̃R�}�ɐi�߂�
	enemyAnimationTimer += deltaTime;
	if (enemyAnimationTimer > 1.0f / animationFPS)
	{
		// �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
		enemyAnimationTimer = 0.0f;
		enemyAnimationNowPattern++;
		if (enemyAnimationNowPattern == 8)
		{
			enemyAnimationNowPattern = 0;
		}

	}

	// �ŏI�I�ȓY�������v�Z 
 // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
	enemyAnimationNowIndex = enemyAnimationNowPattern + enemyAnimationNowType * animationPatternNum;
}

void Enemy::enemyDraw(sHitRect& enemyHit)
{
	//DrawBox(enemyHit.worldLX, enemyHit.worldLY, enemyHit.worldRX, enemyHit.worldRY, enemyColor, true);
	
	
	if (!transparentFlg)
	{
		DrawExtendGraph(pos.x - 180,
			pos.y - 180,
			(pos.x + enemyWidth) + 200,
			(pos.y + enemyWidth) + 155,
			enemyGraph[enemyAnimationNowIndex], true);
	}
	if (transparentFlg && !stopFlg)
	{
		DrawExtendGraph(pos.x - 180,
			pos.y - 180,
			(pos.x + enemyWidth) + 200,
			(pos.y + enemyWidth) + 155,
			enemyTransparentGraph[TransparentAnimationNowIndex], true);
	}
}

