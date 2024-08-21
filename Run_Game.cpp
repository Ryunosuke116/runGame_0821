#include"DxLib.h"
#include"collision_run.h"
#include"player_run.h"
#include"enemy_run.h"
#include"gameStatus.h"
#include"score.h"
#include"backGround.h"
#include"modelMotion.h"


//基本設定
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    //ウィンドウモード
    ChangeWindowMode(false);
    //ウィンドウの命名変更
    SetMainWindowText("RunGame");

    // 垂直同期を待たずにフリップ処理を行う
    SetWaitVSyncFlag(0);

    //ライブラリの初期化
    DxLib_Init();
    //背面画面の初期位置
    //SetWindowInitPosition(0, 0);
    //ウィンドウズサイズとカラーモード
    SetGraphMode(1920, 1080, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    LPCSTR font_path = "font/Nosutaru-dotMPlusH-10-Regular.ttf"; // 読み込むフォントファイルのパス
    if (AddFontResourceEx(font_path, FR_PRIVATE, NULL) > 0) {
    }
    else {
        // フォント読込エラー処理
        MessageBox(NULL, "フォント読込失敗", "", MB_OK);
    }

    int GameStatus = TITLE;


    gameStatusInit();

    //スコア初期化
    scoreUIInit();


    //構造体

    Enemy* enemy = new Enemy();
    sHitRect* enemyHit = new sHitRect;
    enemy->enemyInit(*enemyHit);
    
    Player* player = new Player();

    sHitRect* playerHit = new sHitRect();
    player->playerInit(*playerHit, *enemy);

    model3D* model = new model3D;
    backInit(*model);

    //フレームカウント
    int nowCount, prevCount;
    nowCount = prevCount = GetNowCount();
    //１秒間に60回(FPS回)無限ループ
    while (ProcessMessage() == 0 &&
        CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {

        auto prevTime = GetNowHiPerformanceCount();	// 処理が始まる前の時間

       
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

            //画面の初期化
            ClearDrawScreen();

            backDraw(*enemy,*model);

            player->playerDraw(*playerHit,*enemy,GameStatus);

            enemy->enemyDraw(*enemyHit);

            scoreDraw(*enemy);

            stageDraw();

            // 裏画面の内容を表画面にコピーする（描画の確定）.
            ScreenFlip();

            // 雑なfps固定処理
           // 差を求めて、1回の画面更新が1/60秒になるようにwhileループ回して待つ
            auto afterTime = GetNowHiPerformanceCount(); // 処理が終わった後の時間
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
    //エラーの場合はループを抜ける
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


