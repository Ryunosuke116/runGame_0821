#include"player_run.h"
#include"enemy_run.h"
#include"collision_run.h"
#include"gameStatus.h"


const int animationPatternNum = 10;	//アニメーションのパターン
const int attackPatternNum = 6;	//アニメーションのパターン

const int animationTypeNum = 1;		//アニメーションの種類
const int	ImageSizeY = 160;			//キャラの縦の数値
const int	ImageSizeX = 240;			//キャラの横の数値
const float animationFPS = 3.0f;	//秒間6のアニメーション
const int HP = 3;					//HPの最大値

int playerDeath[animationPatternNum * animationTypeNum];
int animationNowType;					//現在のアニメ種類
int animationNowPattern;					//現在のアニメパターン
int animationNowIndex;					//アニメーション画像配列の添え字0～11までの数値
float animationTimer;				//アニメーションタイマー

int playerRunRight[animationPatternNum * animationTypeNum];
int deathAnimationNowType;					//現在のアニメ種類
int deathAnimationNowPattern;					//現在のアニメパターン
int deathAnimationNowIndex;					//アニメーション画像配列の添え字0～11までの数値
float deathAnimationTimer;				//アニメーションタイマー

int playerAttack[attackPatternNum * animationTypeNum];
int AttackAnimationNowType;					//現在のアニメ種類
int AttackAnimationNowPattern;					//現在のアニメパターン
int AttackAnimationNowIndex;					//アニメーション画像配列の添え字0～11までの数値
float AttackAnimationTimer;				//アニメーションタイマー

int playerHitGraph;

int playerHP[HP * animationTypeNum];

int playerGraph;
int Color = GetColor(255, 0, 0);
int clearStartTime;
int hitStartTime;
int hitSound;
int playerTimeCount;

//HP
int animationHpNowIndex[HP] = { 0 };	 //現在のHPアニメパターン
int HpType = 2;
int HPy = 120;					//HP画像の座標Y

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

    //右に走るモーション
    LoadDivGraph("material/_Run.png",
        animationPatternNum * animationTypeNum,
        animationPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerRunRight);

    //死亡モーション
    LoadDivGraph("material/_Death.png",
        animationPatternNum * animationTypeNum,
        animationPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerDeath);

    //攻撃モーション
    LoadDivGraph("material/_Attack2.png",
        attackPatternNum * animationTypeNum,
        attackPatternNum,
        animationTypeNum,
        ImageSizeX, ImageSizeY,
        playerAttack);

    //ヒットモーション
    playerHitGraph = LoadGraph("material/_Hit.png");

    //HP
    LoadDivGraph("material/Pixel Heart Sprite Sheet 32x32.png",
        HP * animationTypeNum,
        HP, animationTypeNum,
        32, 32,
        playerHP);

    hitSound = LoadSoundMem("sound/打撃1.mp3");


    //HP描画の初期化 
    for (int i = 0; i < HP; i++)
    {
        animationHpNowIndex[i] = 0;
    }

    HpType = 2;			//変更するHP画像の指定

    //プレイヤーの当たり判定矩形の大きさ
    playerGraph = LoadGraph("material/_Idle_1.png");

}


void Player::Update(float deltaTime, 
    Enemy& enemy, sHitRect& playerHit,sHitRect& enemyHit, int& gameStatus)
{

    //playerが初期値からずれている場合徐々に戻す
    if (pos.x < 200.0f && enemy.motionFlg == RUN)
    {
        pos.x += 1.0f;
    }

    //ジャンプ
    if (enemy.motionFlg == RUN && gameStatus != TITLE)
    {
        if (CheckHitKey(KEY_INPUT_SPACE))
        {
            //ジャンプキーが押されてないかつ地面に足がついていればならジャンプする
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

        //空中にいるときの処理
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

    //敵と接触したら終わり
    if (playerHit.isHitRect(playerHit, enemyHit))
    {
        if (!enemy.touchFlg)
        {
            //HPが１減少
            HP -= 1;

            //HP画像を変更
            animationHpNowIndex[HpType] = 2;

            //HpTypeを変更
            HpType -= 1;
           
            enemy.touchFlg = true;
            enemy.motionFlg = HIT;
            hitStartTime = GetNowCount();     //制限時間初期化
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

        //空中にいるときの処理
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

    //敵と接触したら2秒後にリザルト画面に飛ぶ
    if (enemy.motionFlg == RUN)
    {
        clearStartTime = GetNowCount();     //制限時間初期化
        
    }
    else if (enemy.motionFlg == DEATH && !(playerTimeCount <= 0))
    {
        if (!(GetNowCount() - clearStartTime < 2000))
        {
            gameStatus = CLEAR;
        }
    }

    // animTimer で時間経過を測り、 1.0 / animationFPS →次のアニメーションパターンに進んでよい秒数に
       // なったら次のコマに進める
    animationTimer += deltaTime;
    if (animationTimer > 1.0f / animationFPS)
    {
        // タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
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

    // 最終的な添え字を計算 
 // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
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

