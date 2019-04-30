//=====================================
//
//�u���[�t�B���^�[�w�b�_[BlurFilter.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

#include "../main.h"
#include "ScreenObject.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/
class BlurFilter : ScreenObject {
public:
	static BlurFilter* Instance();					//�C���X�^���X�Q�Ə���
	static void Destroy();							//�C���X�^���X�������
	void Draw(UINT pass);							//�`�揈��
	void SetSurfaceSize(float width, float height);	//�T�[�t�F�C�X�T�C�Y�ݒ菈��

private:
	LPD3DXEFFECT effect;							//�V�F�[�_
	D3DXHANDLE texelU, texelV;						//�e�n���h��

	//�V���O���g���C���X�^���X
	static BlurFilter* instance;

	//�V���O���g����
	BlurFilter();
	~BlurFilter();
	BlurFilter(const BlurFilter &) = delete;
	BlurFilter(BlurFilter &&) = delete;
	BlurFilter& operator=(const BlurFilter &) = delete;
	BlurFilter& operator=(BlurFilter &&) = delete;
};

#endif