//=====================================
//
//スパイクノイズコントローラヘッダ[SpikeNoiseController.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _SPIKENOISECONTROLLER_H_
#define _SPIKENOISECONTROLLER_H_

#include "../main.h"
#include "../Framework/BaseSingleton.h"
#include "SpikeNoise.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class SpikeNoiseController : public BaseSingleton<SpikeNoiseController>
{
public:
	friend class BaseSingleton<SpikeNoiseController>;

	void SetNoise(float power, int duration);
	void Update();
	void Draw();

private:
	enum State
	{
		Start,
		Wait,
		End,
		Max
	};

	SpikeNoiseController();
	~SpikeNoiseController();

	SpikeNoise *spikeNoise;
	bool drawFlag;

	bool active;
	int cntFrame;
	int effectTime;
	int state;
	float destPower, srcPower;
};

#endif