//=====================================
//
//ポストエフェクトマネージャ処理[PostEffectManager.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "PostEffectManager.h"

#include "PostEffect\/SpikeNoiseController.h"
#include "PostEffect\ShockBlurController.h"
#include "PostEffect\BloomController.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
インスタンス参照処理
***************************************/
PostEffectManager* PostEffectManager::Instance()
{
	if (instance == NULL)
		instance = new PostEffectManager();

	return instance;
}

/**************************************
インスタンス解放処理
***************************************/
void PostEffectManager::Destroy()
{
	delete instance;
}

/**************************************
コンストラクタ
***************************************/
PostEffectManager::PostEffectManager()
{

}

/**************************************
デストラクタ
***************************************/
PostEffectManager::~PostEffectManager()
{

}

/**************************************
更新処理
***************************************/
void PostEffectManager::Update()
{
	SpikeNoiseController::Instance()->Update();
	ShockBlurController::Instance()->Update();
	BloomController::Instance()->Update();
}

/**************************************
描画処理
***************************************/
void PostEffectManager::Draw()
{
	SpikeNoiseController::Instance()->Draw();
	ShockBlurController::Instance()->Draw();
	BloomController::Instance()->Draw();
}