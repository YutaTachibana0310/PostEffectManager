//=============================================================================
//
// イージングヘッダ [Easing.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EASING_H_
#define _EASING_H_

//*****************************************************************************
// 列挙体定義
//*****************************************************************************
enum EasingType
{
	InCubic,
	OutCubic,
	InOutCubic,
	Linear,
	InExponential,
	OutExponential,
	InOutExponential,
	EasingMax
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float GetEasingValue(float time, float start, float goal, EasingType type);
float EaseInCubic(float time, float start, float goal);			//InCubic計算処理
float EaseOutCubic(float time, float start, float goal);		//OutCubic計算処理
float EaseInOutCubic(float time, float start, float goal);		//InOutCubic計算処理
float EaseLinear(float time, float start, float goal);			//Linear計算処理
float EaseInExponential(float time, float start, float goal);	//InExponential計算処理
float EaseOutExponential(float time, float start, float goal);	//OutExponential計算処理
float EaseInOutExponential(float time, float start, float goal);//InOutExponential計算処理
#endif