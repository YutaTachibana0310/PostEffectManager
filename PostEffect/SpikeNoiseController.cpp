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
	const int Duration = 10;
	const int EaseType[State::Max] = {EasingType::InCubic, EasingType::Linear, EasingType::OutCubic};

	if (!active)
		return;

	cntFrame++;

	//�m�C�Y�̋������A�j���[�V����
	int effectTime = state == State::Wait ? this->effectTime : Duration;
	float t = (float)cntFrame / (float)effectTime;
	float power = Easing<float>::GetEasingValue(t, &this->srcPower, &this->destPower, (EasingType)EaseType[this->state]);
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
�R���X�g���N�^
***************************************/
SpikeNoiseController::SpikeNoiseController()
{
	//�X�p�C�N�m�C�Y�̃C���X�^���X�𐶐�
	spikeNoise = new SpikeNoise();

	//�e�N�X�`�����쐬���T�[�t�F�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &texture, 0);
	texture->GetSurfaceLevel(0, &surface);
}

/**************************************
�f�X�g���N�^
***************************************/
SpikeNoiseController::~SpikeNoiseController()
{
	//���
	delete spikeNoise;
	SAFE_RELEASE(texture);
	SAFE_RELEASE(surface);
}