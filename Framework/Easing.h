//=============================================================================
//
// イージングヘッダ [Easing.h]
// Author : GP12B332 21 立花雄太
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
// クラス宣言
//*****************************************************************************
template<class T>
class Easing
{
public:
	/*****************************************
	イージング計算処理
	引数1 float	time		:経過時間（0.0f ～ 1.0f)
	引数2 T* start			:timeが0の時の値
	引数3 T* goal			:timeが1の時の値
	引数4 EasingType type	:イージングタイプ
	******************************************/
	static T GetEasingValue(float time, const T* start, const T* goal, EasingType type)
	{
		T result;
		switch (type)
		{
		case EasingType::InCubic:
			result = Easing<T>::EaseInCubic(time, start, goal);
			break;

		case EasingType::OutCubic:
			result = Easing<T>::EaseOutCubic(time, start, goal);
			break;

		case EasingType::InOutCubic:
			result = Easing<T>::EaseInOutCubic(time, start, goal);
			break;

		case EasingType::Linear:
			result = Easing<T>::EaseLinear(time, start, goal);
			break;

		case EasingType::InExponential:
			result = Easing<T>::EaseInExponential(time, start, goal);
			break;

		case EasingType::OutExponential:
			result = Easing<T>::EaseOutExponential(time, start, goal);
			break;

		case EasingType::InOutExponential:
			result = Easing<T>::EaseInOutExponential(time, start, goal);
			break;

		default:
			result = Easing<T>::EaseLinear(time, start, goal);
			break;
		}

		return result;
	}

private:
	/***************************
	InCubic計算処理
	***************************/
	static T EaseInCubic(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		return (*goal - *start) * time * time + *start;
	}

	/***************************
	OutCubic計算処理
	***************************/
	static T EaseOutCubic(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		return -(*goal - *start) * time * (time - 2.0f) + *start;
	}

	/***************************
	InOutCubic計算処理
	***************************/
	static T EaseInOutCubic(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		time *= 2.0f;

		if (time < 1)
		{
			return (*goal - *start) / 2.0f * time * time + *start;
		}
		else
		{
			time -= 1.0f;
			return -(*goal - *start) / 2.0f * (time * (time - 2) - 1) + *start;
		}
	}

	/***************************
	Linear計算処理
	***************************/
	static T EaseLinear(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		return (*goal - *start) * time + *start;
	}

	/***************************
	InExponential計算処理
	***************************/
	static T EaseInExponential(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		return (*goal - *start) *  powf(2, 10 * (time - 1)) + *start;
	}

	/***************************
	OutExponential計算処理
	***************************/
	static T EaseOutExponential(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)

			time = 0.0f;
		return (*goal - *start) * (-powf(2, (-10 * time)) + 1) + *start;
	}

	/***************************
	InOutExponential計算処理
	***************************/
	static T EaseInOutExponential(float time, const T* start, const T* goal)
	{
		if (time > 1.0f)
			time = 1.0f;

		if (time < 0.0f)
			time = 0.0f;

		return *start;
	}
};

#endif