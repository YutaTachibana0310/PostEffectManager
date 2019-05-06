//=====================================
//
//カメラシェイカープラグインヘッダ[CameraShakerPlugin.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _CAMERASHAKERPLUGIN_H_
#define _CAMERASHAKERPLUGIN_H_

#include "../main.h"
#include "../camera.h"
#include "BaseCameraPlugin.h"
#include "BaseSingleton.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class CameraShakerPlugin : public BaseCameraPlugin, public BaseSingleton<CameraShakerPlugin>
{
public:
	friend class BaseSingleton<CameraShakerPlugin>;
	void OnUpdate(Camera *entity);
	void Set(float length);

private :
	CameraShakerPlugin() {}
	~CameraShakerPlugin() {}

	int cntFrame;
	float length;
};

#endif