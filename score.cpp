#include"score.h"
#include"collision_run.h"
#include"enemy_run.h"

int scoreGraph;         //スコア描画
int score;              //スコア数
int goodGraph;          //good描画
int comboGraph;         //コンボ描画
int comboCount;         //コンボ数
int bonusCount;         //ボーナスを得られるようになる必要コンボ数
int bonus;              //ボーナススコア
int missGraph;       
int comboBackGraph;
int Transparency;
int evaluationTime;     //評価フラグ
bool evaluationFlg;
bool goodFlg;
bool scoreCountFlg;

void scoreUIInit()
{
    comboCount = 0;
    score = 0;
    bonus = 0; 
    bonusCount = 5;
    evaluationTime = 0;
    Transparency = 100;
    evaluationFlg = false;
    goodFlg = true;
    scoreCountFlg = false;

    scoreGraph = LoadGraph("material/UI_Flat_Button_Large_Release_01a3.png");
    goodGraph = LoadGraph("material/good.png");
    comboGraph = LoadGraph("material/combo.png");
    missGraph = LoadGraph("material/miss.png");
    comboBackGraph = LoadGraph("material/BlackBack.png");
}

void scoreUpdate(sHitRect& playerHit, sHitRect& enemyHit,Enemy& enemy)
{
    //プレイヤーが敵を避けられればコンボ数、スコアが上がる
    if ((playerHit.worldLX >= enemyHit.worldRX) && !enemy.scoreFlag && !enemy.touchFlg)
    {
        if (comboCount >= bonusCount)
        {
            bonusCount += 5;
            bonus += 20;
        }

        score += (SCORE + bonus);
        comboCount += 1;
        evaluationFlg = true;
        enemy.scoreFlag = true;
        goodFlg = true;
    }

    //敵に接触するとコンボとコンボボーナスをリセット
    if (enemy.touchFlg)
    {
        comboCount = 0;
        bonusCount = 5;
        bonus = 0;
        goodFlg = false;
        evaluationFlg = true;
    }
   
}

void scoreDraw(Enemy& enemy)
{
    DrawExtendGraph(0,0,300,110,scoreGraph, true);

   // DrawGraph(1, 0, scoreGraph, true);
    ChangeFont("ノスタルドット（M+）", DX_CHARSET_DEFAULT);
    SetFontSize(80);
    DrawFormatString(100, 10, GetColor(255, 0, 0), "%d",score);
    if (comboCount != 0)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, Transparency);
        DrawGraph(0, 240, comboBackGraph, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        DrawFormatString(100, 250, GetColor(255,0 ,255), "%d",comboCount);
        DrawGraph(190, 250, comboGraph, true);
    }

    //1秒間表示する　
    if (!evaluationFlg)
    {
        evaluationTime = GetNowCount();     //制限時間初期化
    }
    else
    {
        // 現在経過時間を得る
        if ((GetNowCount() - evaluationTime < 1000))
        {
            if (goodFlg)
            {
                DrawGraph(100, 400, goodGraph, true);
            }
            else
            {
                DrawGraph(100, 400, missGraph, true);
            }
        }
        else
        {
            evaluationFlg = false; 
        }
    }
   
}

void scoreCalculation(int& score_result)
{
    if (!scoreCountFlg)
    {
        score_result += score - 200;

        if (score_result < 0)
        {
            score_result = 0;
        }

        scoreCountFlg = true;
    }
    if (score_result < score)
    {
        score_result += 1;
    }
}
void keep(int scoreKeep[])
{
    int tmp = 0;
    //最下位よりスコアが高ければ、更新する
    if (scoreKeep[4] < score)
    {
        scoreKeep[4] = score;

    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = i + 1; j < 5; j++)
        {
            if (scoreKeep[i] < scoreKeep[j])
            {
                tmp = scoreKeep[i];
                scoreKeep[i] = scoreKeep[j];
                scoreKeep[j] = tmp;
            }
        }
    }



}

void combo(int& comboStack)
{
    comboStack = comboCount;
}

