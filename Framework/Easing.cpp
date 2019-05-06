////=============================================================================
////
//// イージング処理 [Easing.cpp]
//// Author : GP12B332 21 立花雄太
////
////=============================================================================
//#include "Easing.h"
//#include <math.h>
//
///*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************/
//
///*****************************************************************************
//イージング計算処理
//*****************************************************************************/
////template<class T>
////T Easing<T>::GetEasingValue(float time, T* start, T* goal, EasingType type)
////{
////	T reuslt;
////	switch (type)
////	{
////	case EasingType::InCubic:
////		result = Easing<T>::EaseInCubic(time, start, goal);
////		break;
////
////	case EasingType::OutCubic:
////		result = Easing<T>::EaseOutCubic(time, start, goal);
////		break;
////
////	case EasingType::InOutCubic:
////		result = Easing<T>::EaseInOutCubic(time, start, goal);
////		break;
////
////	case EasingType::Linear:
////		result = Easing<T>::EaseLinear(time, start, goal);
////		break;
////
////	case EasingType::InExponential:
////		result = Easing<T>::EaseInExponential(time, start, goal);
////		break;
////
////	case EasingType::OutExponential:
////		result = Easing<T>::EaseOutExponential(time, start, goal);
////		break;
////
////	case EasingType::InOutExponential:
////		result = Easing<T>::EaseInOutExponential(time, start, goal);
////		break;
////	}
////
////	return result;
////}
////
///*****************************************************************************
//InCubic計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseInCubic(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	return (goal - start) * time * time + start;
//}
//
///*****************************************************************************
//OutCubic計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseOutCubic(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	return -(goal - start) * time * (time - 2.0f) + start;
//}
//
///*****************************************************************************
//InOutCubic計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseInOutCubic(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	time *= 2.0f;
//
//	if (time < 1)
//	{
//		return (goal - start) / 2.0f * time * time + start;
//	}
//	else
//	{
//		time -= 1.0f;
//		return -(goal - start) / 2.0f * (time * (time - 2) - 1) + start;
//	}
//}
//
///*****************************************************************************
//Linear計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseLinear(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	return (goal - start) * time + start;
//}
//
///*****************************************************************************
//InExponential計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseInExponential(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	return (goal - start) *  powf(2, 10 * (time - 1)) + start;
//}
//
///*****************************************************************************
//OutExponential計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseOutExponential(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//
//		time = 0.0f;
//	return (goal - start) * (-powf(2, (-10 * time)) + 1) + start;
//}
//
///*****************************************************************************
//InOutExponential計算処理
//*****************************************************************************/
//template<class T>
//T Easing<T>::EaseInOutExponential(float time, T* start, T* goal)
//{
//	if (time > 1.0f)
//		time = 1.0f;
//
//	if (time < 0.0f)
//		time = 0.0f;
//
//	return 0.0f;
//}