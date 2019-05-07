//=====================================
//
//スパイクノイズコントローラ処理[SpikeNoiseController.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "SpikeNoiseController.h"
#include "../Framework/Easing.h"

/**************************************
マクロ定義
***************************************/
#define USE_SPIKENOISECTRL_DEBUG

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
セット処理
***************************************/
void SpikeNoiseController::SetNoise(float power, int effectTime)
{
	if (active)
		return;

	this->active = true;
	this->cntFrame = 0;
	this->effectTime = effectTime;
	this->state = State::Start;
	this->destPower = power;
	this->srcPower = 0.0f;
}

/**************************************
更新処理
***************************************/
void SpikeNoiseController::Update()
{
	const int Duration = 10;
	const int EaseType[State::Max] = {EasingType::InCubic, EasingType::Linear, EasingType::OutCubic};

	if (!active)
		return;

	cntFrame++;

	//ノイズの強さをアニメーション
	int effectTime = state == State::Wait ? this->effectTime : Duration;
	float t = (float)cntFrame / (float)effectTime;
	float power = Easing<float>::GetEasingValue(t, &this->srcPower, &this->destPower, (EasingType)EaseType[this->state]);
	this->spikeNoise->SetLength(power);

	//遷移判定
	if (cntFrame == effectTime)
	{
		state++;
		this->cntFrame = 0;
		
		switch (state)
		{
		case State::Wait:
			srcPower = destPower;
			break;

		case State::End:
			destPower = 0;
			break;

		case State::Max:
			active = false;
			break;

		default:
			break;
		}
	}

	//ドローフラグを立てる
	this->drawFlag = true;
}

/**************************************
描画処理
***************************************/
void SpikeNoiseController::Draw()
{
	//非アクティブであれば早期リターン
	if (!this->active)
		return;

	//描画済みであれば早期リターン
	if (!this->drawFlag)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);
	pDevice->SetRenderTarget(0, surface);
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->SetTexture(0, GetCurrentDrawData());
	spikeNoise->DrawEffect();

	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	pDevice->SetTexture(0, texture);
	spikeNoise->Draw();

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

/**************************************
コンストラクタ
***************************************/
SpikeNoiseController::SpikeNoiseController()
{
	//スパイクノイズのインスタンスを生成
	spikeNoise = new SpikeNoise();

	//テクスチャを作成しサーフェイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, 0);
	texture->GetSurfaceLevel(0, &surface);
}

/**************************************
デストラクタ
***************************************/
SpikeNoiseController::~SpikeNoiseController()
{
	//解放
	delete spikeNoise;
	SAFE_RELEASE(texture);
	SAFE_RELEASE(surface);
}