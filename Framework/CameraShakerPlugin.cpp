//=====================================
//
//カメラシェイカープラグイン処理[CameraShakerPlugin.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "CameraShakerPlugin.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
更新処理
***************************************/
void CameraShakerPlugin::OnUpdate(Camera *entity)
{
	const int ShakeDuration = 30;
	const float EndLength = 0.0f;

	cntFrame++;

	float t = (float)cntFrame / (float)ShakeDuration;
	float length = Easing<float>::GetEasingValue(t, &this->length, &EndLength, EasingType::OutCubic);

	D3DXVECTOR3 sideDir;
	D3DXVec3Cross(&sideDir, &(entity->target - entity->pos), &entity->up);
	
	entity->posOffset = sinf(5.0f * cntFrame) * length * entity->up;
	entity->posOffset = cosf(7.5f * cntFrame) * length * sideDir;
}

/**************************************
セット処理
***************************************/
void CameraShakerPlugin::Set(float length)
{
	cntFrame = 0;
	this->length = length;
}