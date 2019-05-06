//=============================================================================
//
// イージングヘッダ [Easing.h]
// Author : GP12B332 21 立花雄太
//
//=============================================================================
#ifndef _EASINGVECTOR_H_
#define _EASINGVECTOR_H_

#include "d3dx9.h"

//*****************************************************************************
// 列挙体定義
//*****************************************************************************
enum EASINGVECTOR_TYPE
{
	InCubic_Vector,
	OutCubic_Vector,
	InOutCubic_Vector,
	Linear_Vector,
	InExponential_Vector,
	OutExponential_Vector,
	InOutExponential_Vector,
	EasingMax_Vector
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXVECTOR3 GetEasingValueVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal, EASINGVECTOR_TYPE type);
D3DXVECTOR3 EaseInCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//InCubic計算処理
D3DXVECTOR3 EaseOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//OutCubic計算処理
D3DXVECTOR3 EaseInOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//InOutCubic計算処理
D3DXVECTOR3 EaseLinearVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//Linear計算処理
D3DXVECTOR3 EaseInExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//InExponential計算処理
D3DXVECTOR3 EaseOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//OutExponential計算処理
D3DXVECTOR3 EaseInOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);//InOutExponential計算処理
#endif