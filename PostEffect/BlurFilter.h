//=====================================
//
//ブラーフィルターヘッダ[BlurFilter.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class BlurFilter : ScreenObject {
public:
	static BlurFilter* Instance();					//インスタンス参照処理
	static void Destroy();							//インスタンス解放処理
	void Draw(UINT pass);							//描画処理
	void SetSurfaceSize(float width, float height);	//サーフェイスサイズ設定処理

private:
	LPD3DXEFFECT effect;							//シェーダ
	D3DXHANDLE texelU, texelV;						//各ハンドル

	//シングルトンインスタンス
	static BlurFilter* instance;

	//シングルトン化
	BlurFilter();
	~BlurFilter();
	BlurFilter(const BlurFilter &) = delete;
	BlurFilter(BlurFilter &&) = delete;
	BlurFilter& operator=(const BlurFilter &) = delete;
	BlurFilter& operator=(BlurFilter &&) = delete;
};

#endif