//=====================================
//
//�u���[���t�B���^�[�w�b�_[BloomFilter.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class BloomFilter : public ScreenObject
{
public:
	BloomFilter();
	~BloomFilter();
	void DrawEffect(UINT pass);
	void SetThrethold(float threthold);
	void SetBloomPower(float power);

private:
	LPD3DXEFFECT effect;
	D3DXHANDLE hThrethold;
	D3DXHANDLE hBloomPower;
};

#endif