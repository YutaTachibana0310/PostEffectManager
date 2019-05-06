//=====================================
//
//ベースカメラプラグインヘッダ[BaseCamerePlugin.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BASECAMERAPLUGIN_H_
#define _BASECAMERAPLUGIN_H_

#include "../camera.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class BaseCameraPlugin
{
public:
	virtual void OnUpdate(Camera *entity) = 0;
	virtual ~BaseCameraPlugin() {};

protected:

};

#endif