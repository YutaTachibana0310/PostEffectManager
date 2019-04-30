//=====================================
//
//ショックブラーコントローラヘッダ[ShockBlurController.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _SHOCKBLURCONTROLLER_H_
#define _SHOCKBLURCONTROLLER_H_

#include "../main.h"
#include "../Framework/BaseSingleton.h"
#include "ShockBlur.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class ShockBlurController : public BaseSingleton<ShockBlurController>
{
public:
	friend class BaseSingleton<ShockBlurController>;

	void Update();
	void Draw();
	void SetBlur(D3DXVECTOR3 pos, float power, int duration);

private:
	enum State
	{
		Start,
		Wait,
		End,
		Max
	};

	ShockBlurController();
	~ShockBlurController();

	ShockBlur *shockBlur;

	bool active;
	bool drawFlag;

	float srcPower, destPower;
	int cntFrame;
	int state;
	int effectTime;
};

#endif