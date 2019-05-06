//=====================================
//
//�u���[���R���g���[���w�b�_[BloomController.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _BLOOMCONTROLLER_H_
#define _BLOOMCONTROLLER_H_

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
class BloomController : public BaseSingleton<BloomController>
{
public:
	friend class BaseSingleton<BloomController>;

	void Update();
	void Draw();

private:
	BloomController();
	~BloomController();

	void SampleBrightness();
	void ProcessBlur();
	void BlendBloom();

	BloomFilter *bloomFilter;
	BlurFilter *blurFilter;

	LPDIRECT3DTEXTURE9 blurTexture[3][2];
	LPDIRECT3DSURFACE9 blurSurface[3][2];
	D3DVIEWPORT9 blurViewPort[3];
	D3DVIEWPORT9 oldViewPort;

	int cntBlur;

};

#endif