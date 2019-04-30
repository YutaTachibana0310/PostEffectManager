//=====================================
//
//ブラーフィルター処理[BlurFilter.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "BlurFilter.h"

/**************************************
マクロ定義
***************************************/
#define EFFECTFILE_BLUR_PATH	"PostEffect/BlurFilter.fx"

#define BLUR_USE_DEBUG

#ifdef BLUR_USE_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
BlurFilter* BlurFilter::instance = NULL;

/**************************************
インスタンス参照処理
***************************************/
BlurFilter* BlurFilter::Instance()
{
	if (BlurFilter::instance == NULL)
		BlurFilter::instance = new BlurFilter();

	return BlurFilter::instance;
}

/**************************************
解放処理
***************************************/
void BlurFilter::Destroy()
{
	delete instance;
}

/**************************************
コンストラクタ
***************************************/
BlurFilter::BlurFilter()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, (LPSTR)EFFECTFILE_BLUR_PATH, 0, 0, 0, 0, &effect, 0);
	texelU = effect->GetParameterByName(0, "texelU");
	texelV = effect->GetParameterByName(0, "texelV");
	effect->SetTechnique("tech");
}

/**************************************
デストラクタ
***************************************/
BlurFilter::~BlurFilter()
{
	SAFE_RELEASE(effect);
}

/**************************************
描画処理
***************************************/
void BlurFilter::Draw(UINT pass)
{
	effect->Begin(0, 0);
	effect->BeginPass(pass);

	ScreenObject::Draw();

	effect->EndPass();
	effect->End();
}

/**************************************
サーフェイスサイズセット処理
***************************************/
void BlurFilter::SetSurfaceSize(float width, float height)
{
	float u[5], v[5];
	for (int i = 0; i < 5; i++)
	{
		u[i] = 1.0f / width * (i + 1);
		v[i] = 1.0f / height * (i + 1);
	}

	effect->SetFloatArray(texelU, u, 5);
	effect->SetFloatArray(texelV, v, 5);
	effect->CommitChanges();

	ScreenObject::Resize(width, height);
}