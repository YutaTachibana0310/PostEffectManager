//=====================================
//
//モノトーンフィルタ処理[MonotoneFilter.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "MonotoneFilter.h"

/**************************************
マクロ定義
***************************************/
#define EFFECTFILE_MONOTONE_PATH	"PostEffect/MonotoneFilter.fx"

#define MONOTONE_USE_DEBUG

#ifdef MONOTONE_USE_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
グローバル変数
***************************************/
MonotoneFilter* MonotoneFilter::instance = NULL;

/**************************************
インスタンス参照処理
***************************************/
MonotoneFilter* MonotoneFilter::Instance()
{
	if (MonotoneFilter::instance == NULL)
		MonotoneFilter::instance = new MonotoneFilter();

	return MonotoneFilter::instance;
}

/**************************************
インスタンス解放処理
***************************************/
void MonotoneFilter::Destroy()
{
	delete MonotoneFilter::instance;
}

/**************************************
コンストラクタ
***************************************/
MonotoneFilter::MonotoneFilter()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, (LPSTR)EFFECTFILE_MONOTONE_PATH, 0, 0, 0, 0, &effect, 0);
	effect->SetTechnique("tech");
	hndlPower = effect->GetParameterByName(0, "power");
}

/**************************************
デストラクタ
***************************************/
MonotoneFilter::~MonotoneFilter()
{
	SAFE_RELEASE(effect);
}

/**************************************
描画処理
***************************************/
void MonotoneFilter::Draw()
{
#ifdef MONOTONE_USE_DEBUG
	BeginDebugWindow("Monotone");
	static float monotonePower = 0.0f;
	DebugSliderFloat("Power", &monotonePower, 0.0f, 1.0f);
	EndDebugWindow("Monotone");
	SetPower(monotonePower);
#endif

	effect->Begin(0, 0);
	effect->BeginPass(0);

	ScreenObject::Draw();

	effect->EndPass();
	effect->End();
}

/**************************************
強さ設定処理
***************************************/
void MonotoneFilter::SetPower(float power)
{
	effect->SetFloat(hndlPower, Clampf(0.0f, 1.0f, power));
	effect->CommitChanges();
}