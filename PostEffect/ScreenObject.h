//=====================================
//
//�X�N���[���I�u�W�F�N�g�w�b�_[ScreenObject.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _SCREENOBJECT_H_
#define _SCREENOBJECT_H_

#include "../main.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class ScreenObject {
public:
	ScreenObject();
	virtual ~ScreenObject();

	void Draw();
	void Resize(float width, float height);

private:
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;
};
/**************************************
�v���g�^�C�v�錾
***************************************/

#endif