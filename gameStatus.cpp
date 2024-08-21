#include"gameStatus.h"
#include"sound.h"


bool pushFlg = false;
bool scoreFlg = false;
bool countFlg = false;
bool keepFlg = false;
bool fixedFlg = false;      //�Œ肷�邩
bool startFlg = false;      //�n�܂邩
bool soundFlg = false;
bool clearFlg = false;
bool isReset = false;

int StartTime = 0;
int startTimeNotice = 0;
//�^�C�g��
int titleGraph;
int wardColor = GetColor(255, 255, 255);
int timer = 0;
int count = 0;
int falling = 0;
int end;
int score_result = 0;
int NoticeCount = 0;
int notice = 0;
int scoreKeep[5];
int ranking;
int y;
int countFrameGraph;
int scoreRankingGraph;
int scoreBackGraph;
int brackGraph;
int ruleGraph;
int startTime = 0;
int clearTime;
int startCount = 3;
int timeCount = 1000;
int brackGraphX;
int whiteGraph;
int alpha, add;

//�T�E���h
int selectSound;             //�Z���N�g���ʉ�
int gameOverBGM;        //�Q�[���I�[�o�[����BGM
int stageBGM;
int resultBGM;

void gameStatusInit()
{
    //�^�C�g��
    titleGraph = LoadGraph("material/title_1.png");
    end = LoadGraph("material/game over.png");
    scoreRankingGraph = LoadGraph("material/score_ranking.png");
    scoreBackGraph = LoadGraph("material/clearscore.png");
    ruleGraph = LoadGraph("material/rule.png");
    brackGraph = LoadGraph("material/wallpaper_00044.png");
    countFrameGraph = LoadGraph("material/Frame_1.png");
    whiteGraph = LoadGraph("material/White.png");
    wardColor = GetColor(255, 255, 255);

    StartTime = 0;
    clearTime = 0;
    startTimeNotice = 0;
    score_result = 0;
    falling = 0;
    notice = 0;
    count = 5;
    NoticeCount = 90 - NOTICE_COUNT;
    timer = 1000;
    y = 0;
    ranking = 1;
    timeCount = 1000;
    startCount = 3;
    brackGraphX = -1980;

    // �A���t�@�l�̏����l�����S�s�����ɂ���
    alpha = 0;
    // �ω��̌������}�C�i�X�ɂ���
    add = 2;

    selectSound = LoadSoundMem("sound/boss_timer2-1.mp3");
    gameOverBGM = LoadSoundMem("sound/iwa_gameover007.mp3");
    stageBGM = LoadSoundMem("sound/maou_bgm_8bit27.mp3");
    resultBGM = LoadSoundMem("sound/�����̃e�[�}.mp3");

    //�t���O�̏�����
    scoreFlg = false;
    countFlg = false;
    keepFlg = false;
    soundFlg = false;
    fixedFlg = false;
    startFlg = false;
    pushFlg = true;
    clearFlg = false;
    isReset = false;
    
}

void title(int& gameStatus,Player& player, sHitRect& playerHit,
           Enemy& enemy, sHitRect& enemyHit, model3D& model)
{
    if (!isReset)
    {
        gameStatusInit();
        scoreUIInit();                           //�X�R�A������
        backInit(model);                              //�w�i������
        player.playerInit(playerHit, enemy);    //�v���C���[�̏�����
        enemy.enemyInit(enemyHit);             //enemy�̏�����
        brackGraphX = -1980;
        isReset = true;
    }

    auto prevTime = GetNowHiPerformanceCount();	// �������n�܂�O�̎���

    player.Update(10.0f / 60.0f, enemy, playerHit, enemyHit, gameStatus);
    backUpdate(enemy);

    //��ʂ̏�����
    ClearDrawScreen();

    backDraw(enemy, model);

    player.playerDraw(playerHit, enemy,gameStatus);

    //�^�C�g��
    DrawGraph(600, 100, titleGraph, true);

    ChangeFont("�m�X�^���h�b�g�iM+�j", DX_CHARSET_DEFAULT);
    SetFontSize(80);
    DrawString(540, 900, "PRESS (SPACE) TO STRAT ", wardColor);

    // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
    ScreenFlip();

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!pushFlg)
        {
            PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
           
            gameStatus = RULE;   
            StartTime = GetNowCount();              //�������ԏ�����
            pushFlg = true;
        }

    }
    else
    {
        pushFlg = false;
    }

    // �G��fps�Œ菈��
    // �������߂āA1��̉�ʍX�V��1/60�b�ɂȂ�悤��while���[�v�񂵂đ҂�
    auto afterTime = GetNowHiPerformanceCount(); // �������I�������̎���
    while (afterTime - prevTime < 16667)
    {
        afterTime = GetNowHiPerformanceCount();
    }

}

void stage(int& gameStatus,Enemy& enemy,model3D& model)
{
    if (enemy.motionFlg == RUN)
    {

        // ���݌o�ߎ��Ԃ𓾂�
        if (!(GetNowCount() - StartTime < timer))
        {
            count -= 1;
            timer += 1000;
        }

    
        //20�b�o������t���O��true�ɂ���noticeCount��20���炷
        if (NoticeCount >= count)
        {
            countFlg = true;
            NoticeCount = NoticeCount - NOTICE_COUNT;
            startTimeNotice = GetNowCount();     //�������ԏ�����
            notice += NOTICE_COUNT;
            enemy.moveSpeed += 5;
            model.flg = true;
        }
    }
    if (count <= 0)
    {
        if (alpha == 0)
        {
            model.motionFlg = true;
        }

        // �A���t�@�l��ω�
        alpha += add;

        // �A���t�@�l�� 0 �� 255 �ɂȂ�����ω��̕����𔽓]����
        if (alpha == 256)
        {
            clearTime = GetNowCount();
        }

        if (!clearFlg)
        {
            clearTime = GetNowCount();     //�������ԏ�����
            clearFlg = true;
            enemy.motionFlg = DEATH;
        }

        if (alpha >= 300)
        {
             brackGraphX += 100;
            if (brackGraphX >= 0)
            {
                brackGraphX = 0;
            }
        }

        if (!(GetNowCount() - clearTime < 3000))
        {
          gameStatus = CLEAR;
        }
    }

}

void stageDraw()
{
    DrawGraph(875, 75, countFrameGraph, true); 
    SetFontSize(100);
    if (count >= 10)
    {
        DrawFormatString(900, 100, GetColor(255, 255, 255), "%d", count);
    }
    else
    {
        DrawFormatString(925, 100, GetColor(255, 255, 255), "%d", count);
    }

    if (clearFlg)
    {
        SetFontSize(200);
        DrawString(700, 300, "�I���I�I", GetColor(255, 0, 0));
    }

    if (countFlg)
    {
        DrawString(900, 940, "�G�̃X�s�[�hUP!", GetColor(255, 0, 0));
        DrawFormatString(900, 840, GetColor(255, 0, 0), "%d�b�o��!!", notice);

        if (!(GetNowCount() - startTimeNotice < 4000))
        {
            countFlg = false;
        }
    }

    // �摜�̃A���t�@�u�����h�ŕ`��
        // ( �`�悵����u�����h���[�h�����ɖ߂� )
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    DrawGraph(0, 0, whiteGraph, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
  
    DrawGraph(brackGraphX, 0, brackGraph, TRUE);
}

void rule(int& gameStatus, Player& player, sHitRect& playerHit,
    Enemy& enemy, sHitRect& enemyHit, model3D model)
{
    if (!fixedFlg)
    {
        //��ʂ̏�����
        ClearDrawScreen();

        backDraw(enemy,model);

        player.playerDraw(playerHit, enemy,gameStatus);

        enemy.enemyDraw(enemyHit);

        scoreDraw(enemy);

        stageDraw();

        ruleDraw();

        // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
        ScreenFlip();
        fixedFlg = true;
    }

    if (!startFlg)
    {
        startTime = GetNowCount();     //�������ԏ�����

        if (CheckHitKey(KEY_INPUT_SPACE))
        {
            if (!pushFlg)
            {
                PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
                startFlg = true;
                pushFlg = true;
            }
        }
        else
        {
            pushFlg = false;
        }
    }
    else if (startFlg)
    {
        //��ʂ̏�����
        ClearDrawScreen();

        backDraw(enemy,model);

        player.playerDraw(playerHit, enemy,gameStatus);

        enemy.enemyDraw(enemyHit);

        scoreDraw(enemy);

        stageDraw();

        SetFontSize(200);
        DrawFormatString(900, 400, GetColor(255, 0, 0), "%d", startCount);

        // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
        ScreenFlip();

        if (!(GetNowCount() - startTime < timeCount))
        {
            startCount--;
            timeCount += 1000;
        }

        if (startCount <= 0)
        {
          
            StartTime = GetNowCount();              //�������ԏ�����
            gameStatus = STAGE;
            playBackBGM(stageBGM);
        }
    }
}

void ruleDraw()
{
    DrawGraph(430, 150, ruleGraph, TRUE);

}

void gameOver(int& gameStatus)
{
    /////////////////////////////////////////
    // �Q�[���I�[�o�[��ʂ̏���
    ////////////////////////////////////////


            //��ʂ̏�����
            ClearDrawScreen();
            if (falling <= 500)
            {
                falling += 1;

            }

            //�t�H���g
            SetFontSize(110);
            DrawString(100, falling, "GAME OVER ", wardColor);

            SetFontSize(60);
            DrawString(670, 800, "PRESS (SPACE) TO TITLE ", wardColor);

            DrawExtendGraph(0, 0, 1920, 600, end, true);

            // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
            ScreenFlip();

            auto prevTime = GetNowHiPerformanceCount();	// �������n�܂�O�̎���


            // �G��fps�Œ菈��
             // �������߂āA1��̉�ʍX�V��1/60�b�ɂȂ�悤��while���[�v�񂵂đ҂�
            auto afterTime = GetNowHiPerformanceCount(); // �������I�������̎���
            while (afterTime - prevTime < 16667)
            {
                afterTime = GetNowHiPerformanceCount();
            }


            if (CheckHitKey(KEY_INPUT_SPACE))
            {
                if (!pushFlg)
                {
                    PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
                    gameStatus = TITLE;
                    StopSoundMem(gameOverBGM);
                    //��ʍX�V����
                    ClearDrawScreen();
                    pushFlg = true;
                }
            }
            else
            {
                pushFlg = false;
            }
            
}

void clear(int& gameStatus)
{
    //BGM���~�߂�
    stopBGM(stageBGM);

    if (!soundFlg)
    {
        playBackBGM(resultBGM);
        soundFlg = true;
    }
  
    brackGraphX += 100;
    if (brackGraphX >= 1980)
    {
        brackGraphX = 1980;
    }

    //��ʂ̏�����
    ClearDrawScreen();

    backDraw();

    //�X�R�A�̕\�������X�ɑ��₷
    scoreCalculation(score_result);

    SetFontSize(600);
    //���ɂ���Ĉʒu�𒲐�
    if (score_result < 100)
    {
        DrawFormatString(700, 100, GetColor(255, 0, 0), "%d", score_result);
    }
    else if (score_result < 1000)
    {
        DrawFormatString(550, 100, GetColor(255, 0, 0), "%d", score_result);
    }
    else if (score_result >= 1000)
    {
        DrawFormatString(350, 100, GetColor(255, 0, 0), "%d", score_result);
    }

    SetFontSize(60);
    DrawString(600, 800, "PRESS (SPACE) TO RANKING ", wardColor);
    
    if (count <= 0)
    {
        DrawGraph(brackGraphX, 0, brackGraph, TRUE);
    }


    // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
    ScreenFlip();

    auto prevTime = GetNowHiPerformanceCount();	// �������n�܂�O�̎���


    // �G��fps�Œ菈��
     // �������߂āA1��̉�ʍX�V��1/60�b�ɂȂ�悤��while���[�v�񂵂đ҂�
    auto afterTime = GetNowHiPerformanceCount(); // �������I�������̎���
    while (afterTime - prevTime < 16667)
    {
        afterTime = GetNowHiPerformanceCount();
    }


    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!pushFlg)
        {
            keep(scoreKeep);
            PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
            gameStatus = RESULT;
            //��ʍX�V����
            ClearDrawScreen();
            pushFlg = true;
        }
    }
    else
    {
        pushFlg = false;
    }
}

void result(int& gameStatus)
{

    //��ʂ̏�����
    ClearDrawScreen();

    backDraw();

    DrawGraph(160, 50, scoreBackGraph, TRUE);
    DrawGraph(600, 100, scoreRankingGraph, TRUE);

    for (int i = 0; i < 5; i++)
    {
        SetFontSize(80);
        DrawFormatString(700, (400 + y), GetColor(255, 255, 255), "%d  %d", ranking, scoreKeep[i]);
        y += 100;
        ranking++;
    }

    y = 0;
    ranking = 1;
    // ����ʂ̓��e��\��ʂɃR�s�[����i�`��̊m��j.
    ScreenFlip();

    auto prevTime = GetNowHiPerformanceCount();	// �������n�܂�O�̎���


    // �G��fps�Œ菈��
     // �������߂āA1��̉�ʍX�V��1/60�b�ɂȂ�悤��while���[�v�񂵂đ҂�
    auto afterTime = GetNowHiPerformanceCount(); // �������I�������̎���
    while (afterTime - prevTime < 16667)
    {
        afterTime = GetNowHiPerformanceCount();
    }

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!pushFlg)
        {
            isReset = false;
            PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
            gameStatus = TITLE;
            stopBGM(resultBGM);
            //��ʍX�V����
            ClearDrawScreen();
            pushFlg = true;
        }
    }
    else
    {
        pushFlg = false;
    }
}

void calculationCount(int& timeCount)
{
    timeCount = count;
}