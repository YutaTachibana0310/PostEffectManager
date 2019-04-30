//=====================================
//
//�V���b�N�u���[�R���g���[������[ShockBlurController.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "ShockBlurController.h"
#include "../Framework//Easing.h"

/**************************************
�}�N����`
***************************************/
#define SHOCKBLURCTRL_USE_DEBUG

#ifdef SHOCKBLURCTRL_USE_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�X�V����
***************************************/
void ShockBlurController::Update()
{
#ifdef SHOCKBLURCTRL_USE_DEBUG
	BeginDebugWindow("ShockBlurCtrl");
	if (DebugButton("Set"))
		SetBlur(D3DXVECTOR3(0.0f, 0.0f, 200.0f), 5.0f, 30);
	EndDebugWindow("ShockBlurCtrl");
#endif // SHOCKBLURCTRL_USE_DEBUG

	const int Duration = 10;
	const int EaseType[State::Max] = { EasingType::InCubic, EasingType::Linear, EasingType::OutCubic };

	//��A�N�e�B�u�ł���Α������^�[��
	if (!active)
		return;

	cntFrame++;

	//�u���[�̋������A�j���[�V����
	int effectDuration = state == State::Wait ? effectTime : Duration;
	float t = (float)cntFrame / (float)effectDuration;
	float power = GetEasingValue(t, srcPower, destPower, (EasingType)EaseType[state]);
	shockBlur->SetPower(power);

	//��ԑJ��
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

	//�h���[�t���O�𗧂Ă�
	drawFlag = true;
}

/**************************************
�`�揈��
***************************************/
void ShockBlurController::Draw()
{
	//��A�N�e�B�u�ł���Α������^�[��
	if (!active)
		return;

	//�`��ς݂ł���Α������^�[��
	if (!drawFlag)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->SetTexture(0, GetCurrentDrawData());

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	shockBlur->Draw();

	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

}

/**************************************
�G�t�F�N�g�Z�b�g����
***************************************/
void ShockBlurController::SetBlur(D3DXVECTOR3 pos, float power, int duration)
{
	//�A�N�e�B�u�ł���ꍇ�͏㏑�����Ȃ�
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
�R���X�g���N�^
***************************************/
ShockBlurController::ShockBlurController()
{
	shockBlur = new ShockBlur();
	shockBlur->SetSurfaceSize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

/**************************************
�f�X�g���N�^
***************************************/
ShockBlurController::~ShockBlurController()
{
	delete shockBlur;
}