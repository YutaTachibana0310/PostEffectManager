//=====================================
//
//�J�����V�F�C�J�[�v���O�C������[CameraShakerPlugin.cpp]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#include "CameraShakerPlugin.h"
#include "Easing.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�X�V����
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
�Z�b�g����
***************************************/
void CameraShakerPlugin::Set(float length)
{
	cntFrame = 0;
	this->length = length;
}