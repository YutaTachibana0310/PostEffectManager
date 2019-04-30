//=====================================
//
//�e���v���[�g�w�b�_[PostEffectManager.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _POSTEFFECTMANAGER_H_
#define _POSTEFFECTMANAGER_H_

#include "main.h"
#include "PostEffect\/ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class PostEffectManager
{
public:
	static PostEffectManager* Instance();
	static void Destroy();
	void Update();
	void Draw();

private:
	//�V���O���g���C���X�^���X
	static PostEffectManager* instance;

	//�V���O���g����
	PostEffectManager();
	~PostEffectManager();
	PostEffectManager(const PostEffectManager &) = delete;
	PostEffectManager(PostEffectManager &&) = delete;
	PostEffectManager& operator=(const PostEffectManager &) = delete;
	PostEffectManager& operator=(PostEffectManager &&) = delete;
};


#endif