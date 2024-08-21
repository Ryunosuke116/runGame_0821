#include"DxLib.h"
#include"collision_run.h"
#include"player_run.h"
#include"enemy_run.h"
#include"gameStatus.h"
#include"score.h"
#include"backGround.h"
#include"modelMotion.h"


//��{�ݒ�
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    //�E�B���h�E���[�h
    ChangeWindowMode(false);
    //�E�B���h�E�̖����ύX
    SetMainWindowText("RunGame");

    // ����������҂����Ƀt���b�v�������s��
    SetWaitVSyncFlag(0);

    //���C�u�����̏�����
    DxLib_Init();
    //�w�ʉ�ʂ̏����ʒu
    //SetWindowInitPosition(0, 0);
    //�E�B���h�E�Y�T�C�Y�ƃJ���[���[�h
    SetGraphMode(1920, 1080, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    LPCSTR font_path = "font/Nosutaru-dotMPlusH-10-Regular.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
    if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
    }
    else {
        // �t�H���g�Ǎ��G���[����
        MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
    }

    int GameStatus = TITLE;


    gameStatusInit();

    //�X�R�A������
    scoreUIInit();


    //�\����

    Enemy* enemy = new Enemy();
    sHitRect* enemyHit = new sHitRect;
    enemy->enemyInit(*enemyHit);
    
    Player* player = new Player();

    sHitRect* playerHit = new sHitRect();
    player->playerInit(*playerHit, *enemy);

    model3D* model = new model3D;
    backInit(*model);

    //�t���[���J�E���g
    int nowCount, prevCount;
    nowCount = prevCount = GetNowCount();
    //�P�b�Ԃ�60��(FPS��)�������[�v
    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {

        auto prevTime = GetNowHiPerformanceCount();	// �������n�܂�O�̎���

       
        if (GameStatus == TITLE)
        {
            title(GameStatus,*player,*playerHit,*enemy,*enemyHit,*model);
            
        }
        else if (GameStatus == RULE)
        {
            rule(GameStatus, *player, *playerHit, *enemy, *enemyHit,*model);

        }
        else if (GameStatus == STAGE)
        {

            player->Update(10.0f/60.0f,*enemy,*playerHit, *enemyHit,GameStatus);

           enemy-> Update(10.0f / 60.0f,*enemyHit);

            scoreUpdate(*playerHit, *enemyHit,*enemy);

            backUpdate(*enemy);
            
            model->modelUpdate();

            stage(GameStatus,*enemy,*model);

            //��ʂ̏�����
            ClearDrawScreen();

            backDraw(*enemy,*model);

            player->playerDraw(*playerHit,*enemy,GameStatus);

            enemy->enemyDraw(*enemyHit);

            scoreDraw(*enemy);

            stageDraw();

            // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
            ScreenFlip();

            // �G��fps�Œ菈��
           // �������߂āA1��̉�ʍX�V��1/60�b�ɂȂ�悤��while���[�v�񂵂đ҂�
            auto afterTime = GetNowHiPerformanceCount(); // �������I�������̎���
            while (afterTime - prevTime < 16667)
            {
                afterTime = GetNowHiPerformanceCount();
            }

        }
        else if (GameStatus == CLEAR)
        {
            clear(GameStatus);
        }
        else if (GameStatus == RESULT)
        {
            result(GameStatus);
        }
    //----------------------------------// 
    //�G���[�̏ꍇ�̓��[�v�𔲂���
    //----------------------------------// 
        if (ProcessMessage() != 0)
        {
            break;
        }
    }

    WaitKey();


    DxLib_End();
    return 0;
}


