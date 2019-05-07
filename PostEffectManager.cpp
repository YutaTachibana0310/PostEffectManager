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
#include "PostEffect\CrossFilterController.h"

/**************************************
マクロ定義
***************************************/
#define POSTEFFECT_USE_DEBUG

#ifdef POSTEFFECT_USE_DEBUG
#include "debugWindow.h"
#endif // POSTEFFECT_USE_DEBUG

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
#ifdef POSTEFFECT_USE_DEBUG
	BeginDebugWindow("PostEffect");

	//ブルーム使用切り替え
	DebugChechBox("User Bloom", &useSceneBloom);

	//クロスフィルタ使用切り替え
	DebugChechBox("Use CrossFilter", &useCrossFilter);

	//スパイクノイズセット
	if(DebugButton("Set SpikeNoise"))
		SpikeNoiseController::Instance()->SetNoise(2.0f, 20);

	//ショックブラーセット
	if(DebugButton("Set ShockBlur"))
		ShockBlurController::Instance()->SetBlur(D3DXVECTOR3(0.0f, 0.0f, 200.0f), 50.0f, 30);

	EndDebugWindow("PostEffect");
#endif // POSTEFFECT_USE_DEBUG


	SpikeNoiseController::Instance()->Update();
	ShockBlurController::Instance()->Update();

	if(useSceneBloom)
		BloomController::Instance()->Update();

	if (useCrossFilter)
		CrossFilterController::Instance()->Update();
}

/**************************************
描画処理
***************************************/
void PostEffectManager::Draw()
{
	SpikeNoiseController::Instance()->Draw();
	ShockBlurController::Instance()->Draw();

	if(useSceneBloom)
		BloomController::Instance()->Draw();

	if (useCrossFilter)
		CrossFilterController::Instance()->Draw();
}