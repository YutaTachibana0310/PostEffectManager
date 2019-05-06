//=====================================
//
//ショックブラーコントローラ処理[ShockBlurController.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "ShockBlurController.h"
#include "../Framework//Easing.h"

/**************************************
マクロ定義
***************************************/
#define SHOCKBLURCTRL_USE_DEBUG

#ifdef SHOCKBLURCTRL_USE_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
更新処理
***************************************/
void ShockBlurController::Update()
{
#ifdef SHOCKBLURCTRL_USE_DEBUG
	BeginDebugWindow("ShockBlurCtrl");
	if (DebugButton("Set"))
		SetBlur(D3DXVECTOR3(0.0f, 0.0f, 200.0f), 50.0f, 30);
	EndDebugWindow("ShockBlurCtrl");
#endif // SHOCKBLURCTRL_USE_DEBUG

	const int Duration = 10;
	const int EaseType[State::Max] = { EasingType::InCubic, EasingType::Linear, EasingType::OutCubic };

	//非アクティブであれば早期リターン
	if (!active)
		return;

	cntFrame++;

	//ブラーの強さをアニメーション
	int effectDuration = state == State::Wait ? effectTime : Duration;
	float t = (float)cntFrame / (float)effectDuration;
	float power = Easing<float>::GetEasingValue(t, &srcPower, &destPower, (EasingType)EaseType[state]);
	shockBlur->SetPower(power);

	//状態遷移
	if (cntFrame == effectDuration)
	{
		state++;
		cntFrame = 0;
		switch (state)
		{
		case State::Wait:
			srcPower = destPower;
			break;

		case State::End:
			destPower = 0.0f;
			break;

		case State::Max:
			active = false;
			break;

		default:
			break;
		}
	}

	//ドローフラグを立てる
	drawFlag = true;
}

/**************************************
描画処理
***************************************/
void ShockBlurController::Draw()
{
	//非アクティブであれば早期リターン
	if (!active)
		return;

	//描画済みであれば早期リターン
	if (!drawFlag)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);
	pDevice->SetRenderTarget(0, surface);
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	pDevice->SetTexture(0, GetCurrentDrawData());
	shockBlur->DrawEffect();

	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	pDevice->SetTexture(0, texture);
	shockBlur->Draw();

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/**************************************
エフェクトセット処理
***************************************/
void ShockBlurController::SetBlur(D3DXVECTOR3 pos, float power, int duration)
{
	//アクティブである場合は上書きしない
	if (active)
		return;

	active = true;
	srcPower = 0.0f;
	destPower = power;
	cntFrame = 0;
	state = State::Start;
	effectTime = duration;

	shockBlur->SetCenterPos(pos);
}

/*************************************
コンストラクタ
***************************************/
ShockBlurController::ShockBlurController()
{
	shockBlur = new ShockBlur();
	shockBlur->SetSurfaceSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, 0);
	texture->GetSurfaceLevel(0, &surface);
}

/**************************************
デストラクタ
***************************************/
ShockBlurController::~ShockBlurController()
{
	delete shockBlur;
	SAFE_RELEASE(texture);
	SAFE_RELEASE(surface);
}