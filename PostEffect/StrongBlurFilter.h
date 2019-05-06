//=====================================
//
//�X�g�����O�u���[�t�B���^�w�b�_[StrongBlurFilter.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _STRONGBLURFILTER_H_
#define _STRONGBLURFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class StrongBlurFilter : public ScreenObject
{
public:	
	StrongBlurFilter();									//�R���X�g���N�^
	~StrongBlurFilter();								//�f�X�g���N�^
	void DrawEffect(UINT pass);							//�`�揈��
	void SetSurfaceSize(float widht, float height);		//�T�[�t�F�C�X�T�C�Y�ݒ菈��

private:
	LPD3DXEFFECT effect;								//�V�F�[�_
	D3DXHANDLE hTexelU, hTexelV;						//�n���h��
};

#endif