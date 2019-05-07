//=====================================
//
//�u���[���R���g���[������[BloomController.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "BloomController.h"

/**************************************
�}�N����`
***************************************/
#define BLOOM_USE_DEBUG

#ifdef BLOOM_USE_DEBUG
#include "../debugWindow.h"
#endif

#define BLOOM_THRETHOLD_DEFAULT (0.2f)

/**************************************
�N���X��`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�X�V����
***************************************/
void BloomController::Update()
{
#ifdef BLOOM_USE_DEBUG
	BeginDebugWindow("Bloom");
	static float threthold = BLOOM_THRETHOLD_DEFAULT;
	DebugSliderFloat("Threthold", &threthold, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
	{
		DebugDrawTexture(blurTexture[i][0], 100.0f, 50.0f);
		DebugSameLine();
	}
	EndDebugWindow("Bloom");
	bloomFilter->SetThrethold(threthold);
#endif // BLOOM_USE_DEBUG

}

/**************************************
�`�揈��
***************************************/
void BloomController::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�u���[����������̂ŃT���v�����O��CLAMP�ɐݒ�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	//Z�o�b�t�@�ɏ������܂Ȃ�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	//�r���[�|�[�g��ޔ�
	pDevice->GetViewport(&oldViewPort);

	//���݂̃����_�[�^�[�Q�b�g��ޔ�
	LPDIRECT3DSURFACE9 oldSurface;
	pDevice->GetRenderTarget(0, &oldSurface);

	//�P�x���o����
	SampleBrightness();

	//���o�����P�x���u���[����
	ProcessBlur();

	//�����_�[�^�[�Q�b�g�����ɖ߂�
	pDevice->SetRenderTarget(0, oldSurface);
	SAFE_RELEASE(oldSurface);

	//�u���[������
	BlendBloom();

	//Z�o�b�t�@�ɏ������ނ悤�ɖ߂�
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	//�T���v�����O�����Ƃɖ߂�
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

/**************************************
�R���X�g���N�^
***************************************/
BloomController::BloomController()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�u���[���t�B���^�̃C���X�^���X�A�e�N�X�`���A�T�[�t�F�C�X��ݒ�
	bloomFilter = new BloomFilter();
	bloomFilter->SetThrethold(BLOOM_THRETHOLD_DEFAULT);

	//�u���[�t�B���^�̃C���X�^���X�A�e�N�X�`���A�T�[�t�F�C�X��ݒ�
	blurFilter = new BlurFilter();
	for (int i = 0; i < 3; i++)
	{
		int reduction = (int)powf(2, i + 2);
		for (int j = 0; j < 2; j++)
		{
			pDevice->CreateTexture(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &blurTexture[i][j], 0);
			blurTexture[i][j]->GetSurfaceLevel(0, &blurSurface[i][j]);
		}

		//�u���[�t�B���^�p�̃r���[�|�[�g��ݒ�
		blurViewPort[i].Width = SCREEN_WIDTH / reduction;
		blurViewPort[i].Height = SCREEN_HEIGHT / reduction;
		blurViewPort[i].X = 0;
		blurViewPort[i].Y = 0;
		blurViewPort[i].MinZ = 0.0f;
		blurViewPort[i].MaxZ = 1.0f;

	}
}

/**************************************
�f�X�g���N�^
***************************************/
BloomController::~BloomController()
{
	delete blurFilter;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			SAFE_RELEASE(blurTexture[j][i]);
			SAFE_RELEASE(blurSurface[j][i]);
		}
	}

	delete bloomFilter;
}

/**************************************
���x���o����
***************************************/
void BloomController::SampleBrightness()
{
	const float BloomPower[3] = { 0.56f, 0.72f, 0.95f };
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < 3; i++)
	{
		//�����_�[�^�[�Q�b�g��ݒ肵�N���A
		pDevice->SetRenderTarget(0, blurSurface[i][0]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

		//�r���[�|�[�g��ݒ�
		pDevice->SetViewport(&blurViewPort[i]);

		//�u���[���̃Q�C����ݒ�
		bloomFilter->SetBloomPower(BloomPower[i]);

		int reduction = (int)powf(2, i + 2);
		bloomFilter->Resize(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction);

		//���݂̕`���񂩂�P�x�𒊏o
		pDevice->SetTexture(0, GetCurrentDrawData());
		bloomFilter->DrawEffect(0);
	}
}

/**************************************
�u���[����
***************************************/
void BloomController::ProcessBlur()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const int PassMax = 2;
	const int TextureMax = 2;

	//�u���[�p�̃T�[�t�F�C�X���N���A
	for (int i = 0; i < 3; i++)
	{
		pDevice->SetRenderTarget(0, blurSurface[i][1]);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);
	}

	for (int j = 0; j < 3; j++)
	{
		//�r���[�|�[�g��ݒ�
		pDevice->SetViewport(&blurViewPort[j]);

		int reduction = (int)powf(2, j + 2);
		blurFilter->SetSurfaceSize(SCREEN_WIDTH / reduction, SCREEN_HEIGHT / reduction);

		cntBlur = 1;

		//�u���[����
		const int BlurLoop = 3;
		for (int i = 0; i < BlurLoop; i++, cntBlur++)
		{
			pDevice->SetRenderTarget(0, blurSurface[j][cntBlur % TextureMax]);
			pDevice->SetTexture(0, blurTexture[j][(cntBlur + 1) % TextureMax]);
			blurFilter->DrawEffect(cntBlur % PassMax);
		}
	}

	//�r���[�|�[�g�����Ƃɖ߂�
	pDevice->SetViewport(&oldViewPort);
}

/**************************************
�u���[����������
***************************************/
void BloomController::BlendBloom()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�|�[�g�����Ƃɖ߂�
	pDevice->SetViewport(&oldViewPort);

	//�����_�[�X�e�[�g�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	bloomFilter->Resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	pDevice->SetTexture(0, blurTexture[0][cntBlur % 2]);
	pDevice->SetTexture(1, blurTexture[1][cntBlur % 2]);
	pDevice->SetTexture(2, blurTexture[2][cntBlur % 2]);

	bloomFilter->DrawEffect(1);

	//�����_�[�X�e�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}