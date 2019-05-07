//=====================================
//
//�N���X�t�B���^�R���g���[���w�b�_[CrossFilterController.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _CROSSFILTERCONTROLLER_H_
#define _CROSSFILTERCONTROLLER_H_

#include "../main.h"
#include "../Framework/BaseSingleton.h"
#include "BloomFilter.h"
#include "BlurFilter.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class CrossFilterController : public BaseSingleton<CrossFilterController>
{
public:
	friend class BaseSingleton<CrossFilterController>;

	void Update();
	void Draw();

private:
	CrossFilterController();
	~CrossFilterController();

	void SampleBrightness();
	void ProcessBlurRightCross();
	void ProcessBlurLeftCross();
	void Blend();

	BloomFilter *bloomFilter;
	BlurFilter *blurFilter;

	LPDIRECT3DTEXTURE9 rightCrossTex[3][2];
	LPDIRECT3DSURFACE9 rightCrossSuf[3][2];
	LPDIRECT3DTEXTURE9 leftCrossTex[3][2];
	LPDIRECT3DSURFACE9 leftCrossSuf[3][2];

	D3DVIEWPORT9 blurViewPort[3];
	D3DVIEWPORT9 oldViewPort;

	int cntBlur;
};

#endif