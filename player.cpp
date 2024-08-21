#include"player_run.h"
#include"enemy_run.h"
#include"collision_run.h"
#include"gameStatus.h"


const int animationPatternNum = 10;	//�A�j���[�V�����̃p�^�[��
const int attackPatternNum = 6;	//�A�j���[�V�����̃p�^�[��

const int animationTypeNum = 1;		//�A�j���[�V�����̎��
const int	ImageSizeY = 160;			//�L�����̏c�̐��l
const int	ImageSizeX = 240;			//�L�����̉��̐��l
const float animationFPS = 3.0f;	//�b��6�̃A�j���[�V����
const int HP = 3;					//HP�̍ő�l

int playerDeath[animationPatternNum * animationTypeNum];
int animationNowType;					//���݂̃A�j�����
int animationNowPattern;					//���݂̃A�j���p�^�[��
int animationNowIndex;					//�A�j���[�V�����摜�z��̓Y����0�`11�܂ł̐��l
float animationTimer;				//�A�j���[�V�����^�C�}�[

int playerRunRight[animationPatternNum * animationTypeNum];
int deathAnimationNowType;					//���݂̃A�j�����
int deathAnimationNowPattern;					//���݂̃A�j���p�^�[��
int deathAnimationNowIndex;					//�A�j���[�V�����摜�z��̓Y����0�`11�܂ł̐��l
float deathAnimationTimer;				//�A�j���[�V�����^�C�}�[

int playerAttack[attackPatternNum * animationTypeNum];
int AttackAnimationNowType;					//���݂̃A�j�����
int AttackAnimationNowPattern;					//���݂̃A�j���p�^�[��
int AttackAnimationNowIndex;					//�A�j���[�V�����摜�z��̓Y����0�`11�܂ł̐��l
float AttackAnimationTimer;				//�A�j���[�V�����^�C�}�[

int playerHitGraph;

int playerHP[HP * animationTypeNum];

int playerGraph;
int Color = GetColor(255, 0, 0);
int clearStartTime;
int hitStartTime;
int hitSound;
int playerTimeCount;

//HP
int animationHpNowIndex[HP] = { 0 };	 //���݂�HP�A�j���p�^�[��
int HpType = 2;
int HPy = 120;					//HP�摜�̍��WY

Player::Player()
{
}

Player::~Player()
{
}

void Player::playerInit(sHitRect& playerHit, Enemy& enemy)
{
    pos = VGet(200.0f, 720.0f, 0.0f);
    jumpFlag = true;
    onGround = true;
    HitonGround = true;
    enemy.motionFlg = RUN;
    currentJumpSpeed = 0;
    H = 0;
    W = 0;
    HP = 3;
    animationNowType = 0;
    animationNowPattern = 0;
    animationNowIndex = 0;
    animationTimer = 0;

    deathAnimationNowType = 0;
    deathAnimationNowPattern = 0;
    deathAnimationNowIndex = 0;
    deathAnimationTimer = 0;

    AttackAnimationNowType = 0;
    AttackAnimationNowPattern = 0;
    AttackAnimationNowIndex = 0;
    AttackAnimationTimer = 0;


    clearStartTime = 0;
    hitStartTime = 0;
    playerTimeCount = 0;

    playerHit.initRect(playerHit, 70,140);

    //�E�ɑ��郂�[�V����
    LoadDivGraph("material/_Run.png",
        animationPatternNum * animationTypeNum,
        animationPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerRunRight);

    //���S���[�V����
    LoadDivGraph("material/_Death.png",
        animationPatternNum * animationTypeNum,
        animationPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerDeath);

    //�U�����[�V����
    LoadDivGraph("material/_Attack2.png",
        attackPatternNum * animationTypeNum,
        attackPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerAttack);

    //�q�b�g���[�V����
    playerHitGraph = LoadGraph("material/_Hit.png");

    //HP
    LoadDivGraph("material/Pixel Heart Sprite Sheet 32x32.png",
        HP * animationTypeNum,
        HP, animationTypeNum,
        32, 32,
        playerHP);

    hitSound = LoadSoundMem("sound/�Ō�1.mp3");


    //HP�`��̏����� 
    for (int i = 0; i < HP; i++)
    {
        animationHpNowIndex[i] = 0;
    }

    HpType = 2;			//�ύX����HP�摜�̎w��

    //�v���C���[�̓����蔻���`�̑傫��
    playerGraph = LoadGraph("material/_Idle_1.png");

}


void Player::Update(float deltaTime, 
    Enemy& enemy, sHitRect& playerHit,sHitRect& enemyHit, int& gameStatus)
{

    //player�������l���炸��Ă���ꍇ���X�ɖ߂�
    if (pos.x < 200.0f && enemy.motionFlg == RUN)
    {
        pos.x += 1.0f;
    }

    //�W�����v
    if (enemy.motionFlg == RUN && gameStatus != TITLE)
    {
        if (CheckHitKey(KEY_INPUT_SPACE))
        {
            //�W�����v�L�[��������ĂȂ����n�ʂɑ������Ă���΂Ȃ�W�����v����
            if (!jumpFlag)
            {
               if (onGround)
               {
                   currentJumpSpeed -= JumpPower;
                   onGround = false;
               }
            }
            jumpFlag = true;
        }
        else
        {
            jumpFlag = false;
        }

        //�󒆂ɂ���Ƃ��̏���
        if (!onGround)
        {
            pos.y += currentJumpSpeed;
            currentJumpSpeed += Gravity;

        }

        if (!onGround)
        {
            if ((pos.y + currentJumpSpeed) > 720.0f)
            {

                currentJumpSpeed = 0.0f;
                onGround = true;
                pos.y = 720.0f;
            }
        }
    }

    playerHit.updateWorldRect(playerHit, pos.x + correX,
        pos.y - correY);

    //�G�ƐڐG������I���
    if (playerHit.isHitRect(playerHit, enemyHit))
    {
        if (!enemy.touchFlg)
        {
            //HP���P����
            HP -= 1;

            //HP�摜��ύX
            animationHpNowIndex[HpType] = 2;

            //HpType��ύX
            HpType -= 1;
           
            enemy.touchFlg = true;
            enemy.motionFlg = HIT;
            hitStartTime = GetNowCount();     //�������ԏ�����
            PlaySoundMem(hitSound, DX_PLAYTYPE_BACK);

        }
    }
    if (HP <= 0)
    {
        enemy.motionFlg = DEATH;
    }

    if (enemy.motionFlg == HIT)
    {
        if (HitonGround)
        {
            currentJumpSpeed -= 10.0f;
            HitonGround = false;
        }

        //�󒆂ɂ���Ƃ��̏���
        if (!HitonGround)
        {
            pos.x -=1.0f;
            pos.y += currentJumpSpeed;
            currentJumpSpeed += Gravity;

        }

        if (!HitonGround)
        {
            if ((pos.y + currentJumpSpeed) > 720.0f)
            {

                currentJumpSpeed = 0.0f;
                HitonGround = true;
                pos.y = 720.0f;
                enemy.motionFlg = RUN;
            }
        }

    }

    //�G�ƐڐG������2�b��Ƀ��U���g��ʂɔ��
    if (enemy.motionFlg == RUN)
    {
        clearStartTime = GetNowCount();     //�������ԏ�����
        
    }
    else if (enemy.motionFlg == DEATH && !(playerTimeCount <= 0))
    {
        if (!(GetNowCount() - clearStartTime < 2000))
        {
            gameStatus = CLEAR;
        }
    }

    // animTimer �Ŏ��Ԍo�߂𑪂�A 1.0 / animationFPS �����̃A�j���[�V�����p�^�[���ɐi��ł悢�b����
       // �Ȃ����玟�̃R�}�ɐi�߂�
    animationTimer += deltaTime;
    if (animationTimer > 1.0f / animationFPS)
    {
        // �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
        animationTimer = 0.0f;
        if (enemy.motionFlg == RUN && !(playerTimeCount <= 0))
        {
            animationNowPattern++;
            if (animationNowPattern == 10)
            {
                animationNowPattern = 0;
            }
        }
        else if (enemy.motionFlg == DEATH && !(playerTimeCount <= 0))
        {
            deathAnimationNowPattern++;

            if (deathAnimationNowPattern >= 9)
            {
               deathAnimationNowPattern = 9;
            }
        }

    }

    // �ŏI�I�ȓY�������v�Z 
 // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
    if (enemy.motionFlg == RUN)
    {
        animationNowIndex = animationNowPattern + 
            animationNowType * animationPatternNum;
    }
    else if (enemy.motionFlg == DEATH)
    {
        deathAnimationNowIndex = deathAnimationNowPattern + 
            deathAnimationNowType * animationPatternNum;
    }

    calculationCount(playerTimeCount);

}

void Player::playerDraw(sHitRect& playerHit, Enemy& enemy,int& gameStatus)
{
    if (enemy.motionFlg == RUN || playerTimeCount <= 0)
    {
        DrawExtendGraph(static_cast<int>(pos.x),
            static_cast<int>(pos.y - 250),
            static_cast<int>(pos.x + 350),
            static_cast<int>(pos.y + 50),
            playerRunRight[animationNowIndex], true);
    }
    else if (enemy.motionFlg == DEATH && !(playerTimeCount <= 0))
    {
        DrawExtendGraph(static_cast<int>(pos.x),
            static_cast<int>(pos.y - 250),
            static_cast<int>(pos.x + 350),
            static_cast<int>(pos.y + 50),
            playerDeath[deathAnimationNowIndex], true);
    }
    else if (enemy.motionFlg == HIT)
    {
        DrawExtendGraph(static_cast<int>(pos.x),
            static_cast<int>(pos.y - 250),
            static_cast<int>(pos.x + 350),
            static_cast<int>(pos.y + 50),
            playerHitGraph, true);
    }
     
    //HP
    if (gameStatus != TITLE)
    {
        DrawExtendGraph(10, HPy, 100, 210, playerHP[animationHpNowIndex[0]], true);
        DrawExtendGraph(100, HPy, 190, 210, playerHP[animationHpNowIndex[1]], true);
        DrawExtendGraph(190, HPy, 280, 210, playerHP[animationHpNowIndex[2]], true);
    }

     // drawRect(playerHit);
    
}

