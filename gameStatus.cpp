#include"gameStatus.h"
#include"sound.h"


bool pushFlg = false;
bool scoreFlg = false;
bool countFlg = false;
bool keepFlg = false;
bool fixedFlg = false;      //固定するか
bool startFlg = false;      //始まるか
bool soundFlg = false;
bool clearFlg = false;
bool isReset = false;

int StartTime = 0;
int startTimeNotice = 0;
//タイトル
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

//サウンド
int selectSound;             //セレクト効果音
int gameOverBGM;        //ゲームオーバー時のBGM
int stageBGM;
int resultBGM;

void gameStatusInit()
{
    //タイトル
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

    // アルファ値の初期値を完全不透明にする
    alpha = 0;
    // 変化の向きをマイナスにする
    add = 2;

    selectSound = LoadSoundMem("sound/boss_timer2-1.mp3");
    gameOverBGM = LoadSoundMem("sound/iwa_gameover007.mp3");
    stageBGM = LoadSoundMem("sound/maou_bgm_8bit27.mp3");
    resultBGM = LoadSoundMem("sound/勝利のテーマ.mp3");

    //フラグの初期化
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
        scoreUIInit();                           //スコア初期化
        backInit(model);                              //背景初期化
        player.playerInit(playerHit, enemy);    //プレイヤーの初期化
        enemy.enemyInit(enemyHit);             //enemyの初期化
        brackGraphX = -1980;
        isReset = true;
    }

    auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

    player.Update(10.0f / 60.0f, enemy, playerHit, enemyHit, gameStatus);
    backUpdate(enemy);

    //画面の初期化
    ClearDrawScreen();

    backDraw(enemy, model);

    player.playerDraw(playerHit, enemy,gameStatus);

    //タイトル
    DrawGraph(600, 100, titleGraph, true);

    ChangeFont("ノスタルドット（M+）", DX_CHARSET_DEFAULT);
    SetFontSize(80);
    DrawString(540, 900, "PRESS (SPACE) TO STRAT ", wardColor);

    // 裏画面の内容を表画面にコピーする（描画の確定）.
    ScreenFlip();

    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!pushFlg)
        {
            PlaySoundMem(selectSound, DX_PLAYTYPE_BACK);
           
            gameStatus = RULE;   
            StartTime = GetNowCount();              //制限時間初期化
            pushFlg = true;
        }

    }
    else
    {
        pushFlg = false;
    }

    // 雑なfps固定処理
    // 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
    auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
    while (afterTime - prevTime < 16667)
    {
        afterTime = GetNowHiPerformanceCount();
    }

}

void stage(int& gameStatus,Enemy& enemy,model3D& model)
{
    if (enemy.motionFlg == RUN)
    {

        // 現在経過時間を得る
        if (!(GetNowCount() - StartTime < timer))
        {
            count -= 1;
            timer += 1000;
        }

    
        //20秒経ったらフラグをtrueにしてnoticeCountを20減らす
        if (NoticeCount >= count)
        {
            countFlg = true;
            NoticeCount = NoticeCount - NOTICE_COUNT;
            startTimeNotice = GetNowCount();     //制限時間初期化
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

        // アルファ値を変化
        alpha += add;

        // アルファ値が 0 か 255 になったら変化の方向を反転する
        if (alpha == 256)
        {
            clearTime = GetNowCount();
        }

        if (!clearFlg)
        {
            clearTime = GetNowCount();     //制限時間初期化
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
        DrawString(700, 300, "終了！！", GetColor(255, 0, 0));
    }

    if (countFlg)
    {
        DrawString(900, 940, "敵のスピードUP!", GetColor(255, 0, 0));
        DrawFormatString(900, 840, GetColor(255, 0, 0), "%d秒経過!!", notice);

        if (!(GetNowCount() - startTimeNotice < 4000))
        {
            countFlg = false;
        }
    }

    // 画像のアルファブレンドで描画
        // ( 描画した後ブレンドモードを元に戻す )
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
        //画面の初期化
        ClearDrawScreen();

        backDraw(enemy,model);

        player.playerDraw(playerHit, enemy,gameStatus);

        enemy.enemyDraw(enemyHit);

        scoreDraw(enemy);

        stageDraw();

        ruleDraw();

        // 裏画面の内容を表画面にコピーする（描画の確定）.
        ScreenFlip();
        fixedFlg = true;
    }

    if (!startFlg)
    {
        startTime = GetNowCount();     //制限時間初期化

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
        //画面の初期化
        ClearDrawScreen();

        backDraw(enemy,model);

        player.playerDraw(playerHit, enemy,gameStatus);

        enemy.enemyDraw(enemyHit);

        scoreDraw(enemy);

        stageDraw();

        SetFontSize(200);
        DrawFormatString(900, 400, GetColor(255, 0, 0), "%d", startCount);

        // 裏画面の内容を表画面にコピーする（描画の確定）.
        ScreenFlip();

        if (!(GetNowCount() - startTime < timeCount))
        {
            startCount--;
            timeCount += 1000;
        }

        if (startCount <= 0)
        {
          
            StartTime = GetNowCount();              //制限時間初期化
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
    // ゲームオーバー画面の処理
    ////////////////////////////////////////


            //画面の初期化
            ClearDrawScreen();
            if (falling <= 500)
            {
                falling += 1;

            }

            //フォント
            SetFontSize(110);
            DrawString(100, falling, "GAME OVER ", wardColor);

            SetFontSize(60);
            DrawString(670, 800, "PRESS (SPACE) TO TITLE ", wardColor);

            DrawExtendGraph(0, 0, 1920, 600, end, true);

            // 裏画面の内容を表画面にコピーする（描画の確定）.
            ScreenFlip();

            auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間


            // 雑なfps固定処理
             // 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
            auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
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
                    //画面更新処理
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
    //BGMを止める
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

    //画面の初期化
    ClearDrawScreen();

    backDraw();

    //スコアの表示を徐々に増やす
    scoreCalculation(score_result);

    SetFontSize(600);
    //桁によって位置を調整
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


    // 裏画面の内容を表画面にコピーする（描画の確定）.
    ScreenFlip();

    auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間


    // 雑なfps固定処理
     // 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
    auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
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
            //画面更新処理
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

    //画面の初期化
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
    // 裏画面の内容を表画面にコピーする（描画の確定）.
    ScreenFlip();

    auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間


    // 雑なfps固定処理
     // 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
    auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
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
            //画面更新処理
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