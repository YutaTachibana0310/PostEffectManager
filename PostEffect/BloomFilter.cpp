//=====================================
//
//ブルームフィルター処理[BloomFilter.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "BloomFilter.h"

/**************************************
マクロ定義
***************************************/
#define EFFECTFILE_BLOOMFILTER_PATH	"PostEffect/BloomFilter.fx"

/**************************************
クラス定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
コンストラクタ
***************************************/
BloomFilter::BloomFilter()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, (LPSTR)EFFECTFILE_BLOOMFILTER_PATH, 0, 0, 0, 0, &effect, 0);
	hThrethold = effect->GetParameterByName(0, "threthold");
	hBloomPower = effect->GetParameterByName(0, "bloomPower");
}

/**************************************
デストラクタ
***************************************/
BloomFilter::~BloomFilter()
{
	SAFE_RELEASE(effect);
}

/**************************************
描画処理
***************************************/
void BloomFilter::DrawEffect()
{
	effect->Begin(0, 0);
	effect->BeginPass(0);

	ScreenObject::Draw();

	effect->EndPass();
	effect->End();
}

/**************************************
しきい値設定処理
***************************************/
void BloomFilter::SetThrethold(float threthold)
{
	effect->SetFloat(hThrethold, threthold);
	effect->CommitChanges();
}

/**************************************
ゲイン設定処理
***************************************/
void BloomFilter::SetBloomPower(float power)
{
	effect->SetFloat(hBloomPower, power);
	effect->CommitChanges();
}