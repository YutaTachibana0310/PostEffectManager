//=====================================
//
//�N���X�u���[�t�B���^�[����[CrossBlurFilter.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "CrossBlurFilter.h"

/**************************************
�}�N����`
***************************************/
#define EFFECTFILE_CROSSBLUR_PATH	"PostEffect/CrossBlur.fx"
#define CROSSBLUR_ARRAY_SIZE		(5)

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�R���X�g���N�^
***************************************/
CrossBlurFilter::CrossBlurFilter()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXCreateEffectFromFile(pDevice, (LPSTR)EFFECTFILE_CROSSBLUR_PATH, 0, 0, 0, 0, &effect, 0);
	hTexelU = effect->GetParameterByName(0, "texelU");
	hTexelV = effect->GetParameterByName(0, "texelV");
	effect->SetTechnique("tech");
}

/**************************************
�f�X�g���N�^
***************************************/
CrossBlurFilter::~CrossBlurFilter()
{
	SAFE_RELEASE(effect);
}

/**************************************
�`�揈��
***************************************/
void CrossBlurFilter::DrawEffect(UINT pass)
{
	effect->Begin(0, 0);
	effect->BeginPass(pass);

	ScreenObject::Draw();

	effect->EndPass();
	effect->End();
}

/**************************************
�T�[�t�F�C�X�T�C�Y�ݒ菈��
***************************************/
void CrossBlurFilter::SetSurfaceSize(float width, float height)
{
	float u[CROSSBLUR_ARRAY_SIZE], v[CROSSBLUR_ARRAY_SIZE];
	
	for(int i = 0; i < CROSSBLUR_ARRAY_SIZE; i++)
	{
		u[i] = 1.0f / width * (i + 1);
		v[i] = 1.0f / height * (i + 1);
	}

	effect->SetFloatArray(hTexelU, u, CROSSBLUR_ARRAY_SIZE);
	effect->SetFloatArray(hTexelV, v, CROSSBLUR_ARRAY_SIZE);
	effect->CommitChanges();

	ScreenObject::Resize(width, height);
}