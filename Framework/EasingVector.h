//=============================================================================
//
// �C�[�W���O�w�b�_ [Easing.h]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#ifndef _EASINGVECTOR_H_
#define _EASINGVECTOR_H_

#include "d3dx9.h"

//*****************************************************************************
// �񋓑̒�`
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
// �v���g�^�C�v�錾
//*****************************************************************************
D3DXVECTOR3 GetEasingValueVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal, EASINGVECTOR_TYPE type);
D3DXVECTOR3 EaseInCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//InCubic�v�Z����
D3DXVECTOR3 EaseOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//OutCubic�v�Z����
D3DXVECTOR3 EaseInOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//InOutCubic�v�Z����
D3DXVECTOR3 EaseLinearVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//Linear�v�Z����
D3DXVECTOR3 EaseInExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//InExponential�v�Z����
D3DXVECTOR3 EaseOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//OutExponential�v�Z����
D3DXVECTOR3 EaseInOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);//InOutExponential�v�Z����
#endif