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
#include "CrossBlurFilter.h"

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
	void Draw(LPDIRECT3DTEXTURE9 targetTexture = NULL);

private:
	CrossFilterController();
	~CrossFilterController();

	void SampleBrightness(LPDIRECT3DTEXTURE9 targetTexture);
	void ProcessBlur(UINT pass);
	void Blend();

	BloomFilter *bloomFilter;
	CrossBlurFilter *blurFilter;

	LPDIRECT3DTEXTURE9 blurTexture[3][2];
	LPDIRECT3DSURFACE9 blurSurface[3][2];

	D3DVIEWPORT9 blurViewPort[3];
	D3DVIEWPORT9 oldViewPort;

	int cntBlur;
};

#endif