//=============================================================================
//
// イージング処理 [Easing.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "Easing.h"
#include <math.h>

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/

/*****************************************************************************
構造体定義
*****************************************************************************/
typedef float(*Easing)(float, float, float);

/*****************************************************************************
グローバル変数
*****************************************************************************/
static Easing EasingProcess[EasingMax] = {
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseLinear,
	EaseInExponential,
	EaseOutExponential,
	EaseInOutExponential
};

/*******************************************************************
//関数名	：float GetEasingValue(float, float, float, float, EASING_TYPE type)
//指定したイージングタイプの値を取得する関数
********************************************************************/
float GetEasingValue(float time, float start, float goal, EasingType type)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return EasingProcess[type](time, start, goal);
}

/*******************************************************************
//関数名	：float EaseInCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInCubic(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) * time * time + start;
}

/*******************************************************************
//関数名	：float EaseOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseOutCubic(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return -(goal - start) * time * (time - 2.0f) + start;
}

/*******************************************************************
//関数名	：float EaseInOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInOutCubic(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	time *= 2.0f;

	if (time < 1)
	{
		return (goal - start) / 2.0f * time * time + start;
	}
	else
	{
		time -= 1.0f;
		return -(goal - start) / 2.0f * (time * (time - 2) - 1) + start;
	}
}

/*******************************************************************
//関数名	：float EaseLinear
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseLinear(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) * time + start;
}

/*******************************************************************
//関数名	：float EaseInExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInExponential(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) *  powf(2, 10 * (time - 1)) + start;
}

/*******************************************************************
//関数名	：float EaseOuntExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseOutExponential(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)

		time = 0.0f;
	return (goal - start) * (-powf(2, (-10 * time)) + 1) + start;
}

/*******************************************************************
//関数名	：float EaseInOutExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInOutExponential(float time, float start, float goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return 0.0f;
}
