//=====================================
//
//�x�[�X�J�����v���O�C���w�b�_[BaseCamerePlugin.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _BASECAMERAPLUGIN_H_
#define _BASECAMERAPLUGIN_H_

#include "../camera.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class BaseCameraPlugin
{
public:
	virtual void OnUpdate(Camera *entity) = 0;
	virtual ~BaseCameraPlugin() {};

protected:

};

#endif