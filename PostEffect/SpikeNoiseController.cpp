//=====================================
//
//�X�p�C�N�m�C�Y�R���g���[������[SpikeNoiseController.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "SpikeNoiseController.h"
#include "../Framework/Easing.h"

/**************************************
�}�N����`
***************************************/
#define USE_SPIKENOISECTRL_DEBUG

#ifdef USE_SPIKENOISECTRL_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�Z�b�g����
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
�X�V����
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

	//�m�C�Y�̋������A�j���[�V����
	int effectTime = state == State::Wait ? this->effectTime : Duration;
	float t = (float)cntFrame / (float)effectTime;
	float power = GetEasingValue(t, this->srcPower, this->destPower, (EasingType)EaseType[this->state]);
	this->spikeNoise->SetLength(power);

	//�J�ڔ���
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

	//�h���[�t���O�𗧂Ă�
	this->drawFlag = true;
}

/**************************************
�`�揈��
***************************************/
void SpikeNoiseController::Draw()
{
	//��A�N�e�B�u�ł���Α������^�[��
	if (!this->active)
		return;

	//�`��ς݂ł���Α������^�[��
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
�R���X�g���N�^
***************************************/
SpikeNoiseController::SpikeNoiseController()
{
	//�X�p�C�N�m�C�Y�̃C���X�^���X�𐶐�
	spikeNoise = new SpikeNoise();
}

/**************************************
�f�X�g���N�^
***************************************/
SpikeNoiseController::~SpikeNoiseController()
{
	//���
	delete spikeNoise;
}