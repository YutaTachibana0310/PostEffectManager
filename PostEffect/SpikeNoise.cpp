//=====================================
//
//�X�p�C�N�m�C�Y����[SpikeNoise.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "SpikeNoise.h"

/**************************************
�}�N����`
***************************************/
#define EFFECT_SPIKENOISE_PATH	"PostEffect/SpikeNoise.fx"

//#define SPIKENOISE_USE_DEBUG

#ifdef SPIKENOISE_USE_DEBUG
#include "../debugWindow.h"
#endif

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�R���X�g���N�^
***************************************/
SpikeNoise::SpikeNoise()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, (LPSTR)EFFECT_SPIKENOISE_PATH, 0, 0, 0, 0, &effect, 0);
	hndlLength = effect->GetParameterByName(0, "length");
}

/**************************************
�f�X�g���N�^
***************************************/
SpikeNoise::~SpikeNoise()
{
	SAFE_RELEASE(effect);
}

/**************************************
�`�揈��
***************************************/
void SpikeNoise::Draw()
{
#ifdef SPIKENOISE_USE_DEBUG
	BeginDebugWindow("SpikeNoise");
	static float length = 0.0f;
	DebugSliderFloat("Length", &length, 0.0f, 1.0f);
	EndDebugWindow("SpikeNoise");
	SetLength(length);
#endif

	effect->Begin(0, 0);
	effect->BeginPass(0);

	ScreenObject::Draw();

	effect->EndPass();
	effect->End();
}

/**************************************
�c�ݒ����Z�b�g����
***************************************/
void SpikeNoise::SetLength(float length)
{
	effect->SetFloat(hndlLength, length);
	effect->CommitChanges();
}