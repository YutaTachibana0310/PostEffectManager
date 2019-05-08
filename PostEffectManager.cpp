//=====================================
//
//�|�X�g�G�t�F�N�g�}�l�[�W������[PostEffectManager.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "PostEffectManager.h"

#include "PostEffect\/SpikeNoiseController.h"
#include "PostEffect\ShockBlurController.h"
#include "PostEffect\BloomController.h"
#include "PostEffect\CrossFilterController.h"

/**************************************
�}�N����`
***************************************/
#define POSTEFFECT_USE_DEBUG

#ifdef POSTEFFECT_USE_DEBUG
#include "debugWindow.h"
#endif // POSTEFFECT_USE_DEBUG

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
	useSceneBloom = true;
	useCrossFilter = false;
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
#ifdef POSTEFFECT_USE_DEBUG
	BeginDebugWindow("PostEffect");

	//�u���[���g�p�؂�ւ�
	DebugChechBox("User Bloom", &useSceneBloom);

	//�N���X�t�B���^�g�p�؂�ւ�
	DebugChechBox("Use CrossFilter", &useCrossFilter);

	//�X�p�C�N�m�C�Y�Z�b�g
	if(DebugButton("Set SpikeNoise"))
		SpikeNoiseController::Instance()->SetNoise(2.0f, 20);

	//�V���b�N�u���[�Z�b�g
	if(DebugButton("Set ShockBlur"))
		ShockBlurController::Instance()->SetBlur(D3DXVECTOR3(0.0f, 0.0f, 200.0f), 50.0f, 30);

	EndDebugWindow("PostEffect");
#endif // POSTEFFECT_USE_DEBUG


	SpikeNoiseController::Instance()->Update();
	ShockBlurController::Instance()->Update();

	if(useSceneBloom)
		BloomController::Instance()->Update();

	if (useCrossFilter)
		CrossFilterController::Instance()->Update();
}

/**************************************
�`�揈��
***************************************/
void PostEffectManager::Draw()
{
	SpikeNoiseController::Instance()->Draw();
	ShockBlurController::Instance()->Draw();

	if(useSceneBloom)
		BloomController::Instance()->Draw();

	if (useCrossFilter)
		CrossFilterController::Instance()->Draw();
}