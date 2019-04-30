//=====================================
//
//スパイクノイズヘッダ[SpikeNoiise.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _SPIKENOISE_H_
#define _SPIKENOISE_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class SpikeNoise :public ScreenObject
{
public:
	SpikeNoise();
	~SpikeNoise();
	void Draw();
	void SetLength(float length);

private:
	LPD3DXEFFECT effect;
	D3DXHANDLE hndlLength;
};


#endif