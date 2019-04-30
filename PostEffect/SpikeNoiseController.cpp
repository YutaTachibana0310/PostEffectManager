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

#ifdef USE_SPIKENOISECTRL_DEBUG
#include "../debugWindow.h"
#endif

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
#ifdef USE_SPIKENOISECTRL_DEBUG
	BeginDebugWindow("SpikeNoiseCtrl");
	if (DebugButton("Set"))
		SetNoise(2.0f, 20);
	EndDebugWindow("SpikeNoiseCtrl");
#endif

	const int Duration = 10;
	const int EaseType[State::Max] = {EasingType::InCubic, EasingType::Linear, EasingType::OutCubic};

	if (!active)
		return;

	cntFrame++;

	//ノイズの強さをアニメーション
	int effectTime = state == State::Wait ? this->effectTime : Duration;
	float t = (float)cntFrame / (float)effectTime;
	float power = GetEasingValue(t, this->srcPower, this->destPower, (EasingType)EaseType[this->state]);
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

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	pDevice->SetTexture(0, GetCurrentDrawData());
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
}

/**************************************
デストラクタ
***************************************/
SpikeNoiseController::~SpikeNoiseController()
{
	//解放
	delete spikeNoise;
}