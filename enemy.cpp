#include"enemy_run.h"
#include"collision_run.h"
#include"score.h"

const int	ImageSizeY = 150;			//キャラの縦の数値
const int	ImageSizeX = 150;			//キャラの横の数値
const int enemyWidth = 80;
const int animationPatternNum = 8;	//アニメーションのパターン
const int TransparentAnimationPatternNum = 4;
const int animationTypeNum = 1;		//アニメーションの種類
const float animationFPS = 6.0f;	//秒間6のアニメーション

int enemyColor = GetColor(255, 255, 0);

int enemyGraph[animationPatternNum * animationTypeNum];
int enemyAnimationNowType;					//現在のアニメ種類
int enemyAnimationNowPattern;					//現在のアニメパターン
int enemyAnimationNowIndex;					//アニメーション画像配列の添え字0～11までの数値
float enemyAnimationTimer;				//アニメーションタイマー

int enemyTransparentGraph[TransparentAnimationPatternNum * animationTypeNum];
int TransparentAnimationNowType;			//現在のアニメ種類
int TransparentAnimationNowPattern;			//現在のアニメパターン
int TransparentAnimationNowIndex;			//アニメーション画像配列の添え字0～11までの数値
float TransparentAnimationTimer;			//アニメーションタイマー


int enemyCollision;
int comboStack;

bool transparentFlg = false;		//モーション変更
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
	
	//正規化
	if (inputVec.x != 0)
	{
		inputVec = VNorm(inputVec);
	}

	//移動量計算
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

	//10コンボ以上のとき、透明になるモーション
	if (comboStack >= 10 && backFlg)
	{
		//1400の地点で透明になる
		if (pos.x <= 1400 && pos.x >= 1300)
		{
			if (!stopFlg)
			{
				pos.x = 1400;
			}

			transparentFlg = true;

			// なったら次のコマに進める
			TransparentAnimationTimer += deltaTime;
			if (TransparentAnimationTimer > 1.0f / animationFPS+1.0f)
			{
				// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
				TransparentAnimationTimer = 0.0f;
				TransparentAnimationNowPattern--;
				if (TransparentAnimationNowPattern <= 0)
				{
					stopFlg = true;
					TransparentAnimationNowPattern = 0;
				}
			}

		}

	// 最終的な添え字を計算 
	 // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
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

			// なったら次のコマに進める
			TransparentAnimationTimer += deltaTime;
			if (TransparentAnimationTimer > 1.0f / animationFPS + 1.0f)
			{
				// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
				TransparentAnimationTimer = 0.0f;
				TransparentAnimationNowPattern++;
				if (TransparentAnimationNowPattern >= 3)
				{
					backFlg = false;
					stopFlg = true;
					TransparentAnimationNowPattern = 3;
				}
			}

			// 最終的な添え字を計算 
	 // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
			TransparentAnimationNowIndex = TransparentAnimationNowPattern +
				TransparentAnimationNowType * TransparentAnimationPatternNum;
		}
	}

	//700の地点で見えるようになる
	if (pos.x <= 700)
	{
		transparentFlg = false;
		backFlg = false;
	}


	// なったら次のコマに進める
	enemyAnimationTimer += deltaTime;
	if (enemyAnimationTimer > 1.0f / animationFPS)
	{
		// タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
		enemyAnimationTimer = 0.0f;
		enemyAnimationNowPattern++;
		if (enemyAnimationNowPattern == 8)
		{
			enemyAnimationNowPattern = 0;
		}

	}

	// 最終的な添え字を計算 
 // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
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

