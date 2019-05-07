//=====================================
//
//�N���X�t�B���^�R���g���[������[CrossFilterController.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "CrossFilterController.h"

/**************************************
�}�N����`
***************************************/
#define CROSSFILTER_USE_DEBUG

#ifdef CROSSFILTER_USE_DEBUG
#include "../debugWindow.h"
#endif // CROSSFILTER_USE_DEBUG

#define CROSSFILTER_THRETHOLD_DEFAULT	(0.2f)

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�X�V����
***************************************/
void CrossFilterController::Update()
{
#ifdef CROSSFILTER_USE_DEBUG
	BeginDebugWindow("CorssFilter");
	static float threthold = CROSSFILTER_THRETHOLD_DEFAULT;
	DebugSliderFloat("Threthold", &threthold, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		DebugDrawTexture(rightCrossTex[i][0], 100.0f, 50.0f);
		DebugSameLine();
	}
	for (int i = 0; i < 3; i++)
	{
		DebugDrawTexture(leftCrossTex[i][0], 100.0f, 50.0f);
		DebugSameLine();
	}
	EndDebugWindow("CrossFilter");
	bloomFilter->SetThrethold(threthold);
#endif // CROSSFILTER_USE_DEBUG

}

/**************************************
�`�揈��
***************************************/
void CrossFilterController::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�u���[����������̂ŃT���v�����O��CLAMP�ɐݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//Z�o�b�t�@�ւ̏������݂��s��Ȃ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//�r���[�|�[�g��ޔ�
	pDevice->GetViewport(&oldViewPort);

	//�����_�[�^�[�Q�b�g��ޔ�
	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);

	//�P�x���o����
	SampleBrightness();

	//���o�����P�x���u���[����(
	ProcessBlurRightCross();
	ProcessBlurLeftCross();

	//�����_�[�^�[�Q�b�g�����ɖ߂�
	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	//�N���X�t�B���^����
	Blend();

	//Z�o�b�t�@�ɏ������ނ悤�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//�T���v�����O�����ɖ߂�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

/**************************************
�R���X�g���N�^
***************************************/
CrossFilterController::CrossFilterController()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�u���[���t�B���^�̃C���X�^���X�A�e�N�X�`���A�T�[�t�F�C�X��ݒ�
	bloomFilter = new BloomFilter();
	bloomFilter->SetThrethold(CROSSFILTER_THRETHOLD_DEFAULT);

	//�u���[�t�B���^�̃C���X�^���X�𐶐�
	blurFilter = new BlurFilter();

	//�N���X�t�B���^�p�̃e�N�X�`�����쐬�i�P�j
	for (int i = 0; i < 3; i++)
	{
		int reduction = (int)powf(2.0f, i + 2.0f);
		for (int j = 0; j < 2; j++)
		{
			pDevice->CreateTexture(SCREEN_WIDTH / reduction,
				SCREEN_HEIGHT / reduction,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_X8R8G8B8,
				D3DPOOL_DEFAULT,
				&rightCrossTex[i][j],
				0);
			rightCrossTex[i][j]->GetSurfaceLevel(0, &rightCrossSuf[i][j]);
		}
	}

	//�N���X�t�B���^�p�̃e�N�X�`�����쐬�i�Q�j
	for (int i = 0; i < 3; i++)
	{
		int reduction = (int)powf(2.0f, i + 2.0f);
		for (int j = 0; j < 2; j++)
		{
			pDevice->CreateTexture(SCREEN_WIDTH / reduction,
				SCREEN_HEIGHT / reduction,
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_X8R8G8B8,
				D3DPOOL_DEFAULT,
				&leftCrossTex[i][j],
				0);
			leftCrossTex[i][j]->GetSurfaceLevel(0, &leftCrossSuf[i][j]);
		}
	}

	//�k���o�b�t�@�p�̃r���[�|�[�g���쐬
	for (int i = 0; i < 3; i++)
	{
		int reduction = (int)powf(2.0f, i + 2.0f);
		blurViewPort[i].Width = SCREEN_WIDTH / reduction;
		blurViewPort[i].Height = SCREEN_HEIGHT / reduction;
		blurViewPort[i].MinZ = 0.0f;
		blurViewPort[i].MaxZ = 1.0f;
		blurViewPort[i].X = 0;
		blurViewPort[i].Y = 0;
	}
}

/**************************************
�f�X�g���N�^
***************************************/
CrossFilterController::~CrossFilterController()
{
	delete blurFilter;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			SAFE_RELEASE(rightCrossTex[i][j]);
			SAFE_RELEASE(rightCrossSuf[i][j]);
			SAFE_RELEASE(leftCrossTex[i][j]);
			SAFE_RELEASE(leftCrossSuf[i][j]);
		}
	}

	delete bloomFilter;
}

/**************************************
�P�x���o����
***************************************/
void CrossFilterController::SampleBrightness()
{
	const float BloomPower[3] = { 0.56f, 0.72f, 0.95f };
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < 3; i++)
	{
		//�r���[�|�[�g��ݒ�
		pDevice->SetViewport(&blurViewPort[i]);

		//�Q�C����ݒ�
		bloomFilter->SetBloomPower(BloomPower[i]);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, GetCurrentDrawData());

		//�k���o�b�t�@�p�ɒ��_�����T�C�Y
		float reduction = powf(2.0f, i + 2.0f);
		bloomFilter->Resize(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction);

		//�����_�[�^�[�Q�b�g��ݒ� 
		pDevice->SetRenderTarget(0, rightCrossSuf[i][0]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

		//���݂̕`���񂩂�P�x�𒊏o�i�P�j
		bloomFilter->DrawEffect(0);

		//�����_�[�^�[�Q�b�g��ݒ�
		pDevice->SetRenderTarget(0, leftCrossSuf[i][0]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

		//�P�x�𒊏o�i�Q�j
		bloomFilter->DrawEffect(0);
	}
}

/**************************************
�u���[����
***************************************/
void CrossFilterController::ProcessBlurRightCross()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const int StartPass = 2, PassMax = 2;
	const int TextureMax = 2;

	//�u���[�p�̃T�[�t�F�C�X���N���A(1)
	for (int i = 0; i < 3; i++)
	{
		pDevice->SetRenderTarget(0, rightCrossSuf[i][1]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);
	}

	for (int i = 0; i < 3; i++)
	{
		//�r���[�|�[�g��ݒ�
		pDevice->SetViewport(&blurViewPort[i]);

		float reduction = powf(2.0f, i + 2.0f);
		blurFilter->SetSurfaceSize(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction);

		cntBlur = 1;

		//�u���[����
		const int BlurLoop = 3;
		for (int j = 0; j < BlurLoop; j++, cntBlur++)
		{
			pDevice->SetRenderTarget(0, rightCrossSuf[i][cntBlur % TextureMax]);
			pDevice->SetTexture(0, rightCrossTex[i][(cntBlur + 1) % TextureMax]);
			blurFilter->DrawEffect(cntBlur % PassMax + StartPass);
		}
	}

	//�r���[�|�[�g��ݒ�ɖ߂�
	pDevice->SetViewport(&oldViewPort);
}

/**************************************
�u���[����
***************************************/
void CrossFilterController::ProcessBlurLeftCross()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const int StartPass = 2, PassMax = 2;
	const int TextureMax = 2;

	//�u���[�p�̃T�[�t�F�C�X���N���A(1)
	for (int i = 0; i < 3; i++)
	{
		pDevice->SetRenderTarget(0, leftCrossSuf[i][1]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);
	}

	for (int i = 0; i < 3; i++)
	{
		//�r���[�|�[�g��ݒ�
		pDevice->SetViewport(&blurViewPort[i]);

		float reduction = powf(2.0f, i + 2.0f);
		blurFilter->SetSurfaceSize(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction);

		cntBlur = 1;

		//�u���[����
		const int BlurLoop = 3;
		for (int j = 0; j < BlurLoop; j++, cntBlur++)
		{
			pDevice->SetRenderTarget(0, leftCrossSuf[i][cntBlur % TextureMax]);
			pDevice->SetTexture(0, leftCrossTex[i][(cntBlur + 1) % TextureMax]);
			blurFilter->DrawEffect(cntBlur % PassMax + StartPass);
		}
	}

	//�r���[�|�[�g��ݒ�ɖ߂�
	pDevice->SetViewport(&oldViewPort);
}

/**************************************
��������
***************************************/
void CrossFilterController::Blend()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�|�[�g�����ɖ߂�
	pDevice->SetViewport(&oldViewPort);

	//�����_�[�X�e�[�g�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�u���[�����������ʂ����Z�����i�P�j
	bloomFilter->Resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	pDevice->SetTexture(0, rightCrossTex[0][cntBlur % 2]);
	pDevice->SetTexture(1, rightCrossTex[1][cntBlur % 2]);
	pDevice->SetTexture(2, rightCrossTex[2][cntBlur % 2]);

	bloomFilter->DrawEffect(1);

	//�u���[�����������ʂ����Z�����i�Q�j
	pDevice->SetTexture(0, leftCrossTex[0][cntBlur % 2]);
	pDevice->SetTexture(1, leftCrossTex[1][cntBlur % 2]);
	pDevice->SetTexture(2, leftCrossTex[2][cntBlur % 2]);

	bloomFilter->DrawEffect(1);

	//�����_�[�X�e�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}