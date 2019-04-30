//=====================================
//
//���m�g�[���t�B���^�w�b�_[MonotoneFilter.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _MONOTONEFILTER_H_
#define _MONOTONEFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class MonotoneFilter : ScreenObject {
public:
	static MonotoneFilter* Instance();				//�C���X�^���X�Q�Ə���
	static void Destroy();							//�C���X�^���X�������
	void Draw();									//�`�揈��
	void SetPower(float power);						//�����ݒ菈��

private:
	LPD3DXEFFECT effect;							//�V�F�[�_
	D3DXHANDLE hndlPower;							//�e�n���h��

	//�V���O���g���C���X�^���X
	static MonotoneFilter* instance;

	//�V���O���g����
	MonotoneFilter();
	~MonotoneFilter();
	MonotoneFilter(const MonotoneFilter &) = delete;
	MonotoneFilter(MonotoneFilter &&) = delete;
	MonotoneFilter& operator=(const MonotoneFilter &) = delete;
	MonotoneFilter& operator=(MonotoneFilter &&) = delete;
};

#endif