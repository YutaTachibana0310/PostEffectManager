//=============================================================================
//
// �C�[�W���O���� [Easing.cpp]
// Author : GP11A341 21 ���ԗY��
//
//=============================================================================
#include "EasingVector.h"
#include <math.h>

/*****************************************************************************
�v���g�^�C�v�錾
*****************************************************************************/

/*****************************************************************************
�\���̒�`
*****************************************************************************/
typedef D3DXVECTOR3(*Easing)(float, D3DXVECTOR3, D3DXVECTOR3);

/*****************************************************************************
�O���[�o���ϐ�
*****************************************************************************/
static Easing EasingProcess[EasingMax_Vector] = {
	EaseInCubicVector,
	EaseOutCubicVector,
	EaseInOutCubicVector,
	EaseLinearVector,
	EaseInExponentialVector,
	EaseOutExponentialVector,
	EaseInOutExponentialVector
};

/*******************************************************************
//�֐���	�FD3DXVECTOR3 GetEasingValue(D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR3, EASINGVECTOR_TYPE type)
//�w�肵���C�[�W���O�^�C�v�̒l���擾����֐�
********************************************************************/
D3DXVECTOR3 GetEasingValueVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal, EASINGVECTOR_TYPE type)
{
	return EasingProcess[type](time, start, goal);
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseInCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseInCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) * time * time + start;
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseOutCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return -(goal - start) * time * (time - 2.0f) + start;
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseInOutCubic
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseInOutCubicVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
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
//�֐���	�FD3DXVECTOR3 EaseLinear
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseLinearVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) * time + start;
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseInExponential
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseInExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) *  powf(2, 10 * (time - 1)) + start;
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseOuntExponential
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	return (goal - start) * (-powf(2, (-10 * time)) + 1) + start;
}

/*******************************************************************
//�֐���	�FD3DXVECTOR3 EaseInOutExponential
//����1		�Ffloat time		�F���݂̐i�s�x
//����2		�FD3DXVECTOR3 start		�F�����l
//����3		�FD3DXVECTOR3 goal		�F�ڕW�l
//����4		�FD3DXVECTOR3 duration	�F�i�s�x�̍̏W�n
//�߂�l	�F�C�[�W���O�̒l
//����		�F�C�[�W���O�v�Z����
********************************************************************/
D3DXVECTOR3 EaseInOutExponentialVector(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal)
{
	if (time > 1.0f)
		time = 1.0f;

	if (time < 0.0f)
		time = 0.0f;

	time *= 2.0f;

	if (time < 1)
	{
		return (goal - start) / 2.0f *  powf(2, 10 * (time - 1)) + start;
	}
	else
	{
		time -= 1.0f;
		return (goal - start) / 2.0f * (-powf(2, (-10 * time)) + 2) + start;
	}
}