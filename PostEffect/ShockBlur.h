//=====================================
//
//�V���b�N�u���[�w�b�_[ShockBlur.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _SHOCKBLUR_H_
#define _SHOCKBLUR_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class ShockBlur : public ScreenObject
{
public:
	ShockBlur();
	~ShockBlur();
	void DrawEffect();
	void SetPower(float power);
	void SetCenterPos(D3DXVECTOR3 pos);
	void SetSurfaceSize(float widht, float height);

private:
	LPD3DXEFFECT effect;				//�V�F�[�_
	D3DXHANDLE hndlPower, hndlCenter;	//�e�n���h��
	D3DXHANDLE hndlTU, hndlTV;			//�e�n���h��
};

#endif