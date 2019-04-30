//=====================================
//
//�|�X�g�G�t�F�N�g�}�l�[�W������[PostEffectManager.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "PostEffectManager.h"

#include "PostEffect\/SpikeNoiseController.h"
#include "PostEffect\ShockBlurController.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�C���X�^���X�Q�Ə���
***************************************/
PostEffectManager* PostEffectManager::Instance()
{
	if (instance == NULL)
		instance = new PostEffectManager();

	return instance;
}

/**************************************
�C���X�^���X�������
***************************************/
void PostEffectManager::Destroy()
{
	delete instance;
}

/**************************************
�R���X�g���N�^
***************************************/
PostEffectManager::PostEffectManager()
{

}

/**************************************
�f�X�g���N�^
***************************************/
PostEffectManager::~PostEffectManager()
{

}

/**************************************
�X�V����
***************************************/
void PostEffectManager::Update()
{
	SpikeNoiseController::Instance()->Update();
	ShockBlurController::Instance()->Update();
}

/**************************************
�`�揈��
***************************************/
void PostEffectManager::Draw()
{

	SpikeNoiseController::Instance()->Draw();
	ShockBlurController::Instance()->Draw();
}