//=====================================
//
//�N���X�u���[�t�B���^�[�w�b�_[CrossBlurFilter.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _CROSSBLURFILTER_H_
#define _CROSSBLURFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class CrossBlurFilter : public ScreenObject
{
public:
	CrossBlurFilter();
	~CrossBlurFilter();
	void DrawEffect(UINT pass);
	void SetSurfaceSize(float width, float height);

private:
	LPD3DXEFFECT effect;
	D3DXHANDLE hTexelU, hTexelV;
};

#endif