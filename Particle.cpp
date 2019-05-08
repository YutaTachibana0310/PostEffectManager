#include "Particle.h"
#include "debugWindow.h"
#include "PostEffect\CrossFilterController.h"
#include "PostEffect\ScreenObject.h"

/************************************
�}�N����`
*************************************/
#define PARTICLE_NUM			(2048)
#define PARTICLE_POS_X_MAX		(500.0f)
#define PARTICLE_POS_Y_MAX		(200.0f)
#define PARTICLE_POS_Z_MAX		(2000.0f)
#define PARTICLE_MOVE_SPEED		(0.5f)
#define PARTICLE_SIZE			(5.0f)

/************************************
�v���g�^�C�v�錾
*************************************/
void DrawColorAndBloom();

/************************************
�O���[�o���ϐ�
*************************************/
static Transform trasnform;
static ParticleTex uv;

//�p�[�e�B�N���̒P�ʒ��_
static ParticleVertex vtx[4] =
{
	{ D3DXVECTOR3(-PARTICLE_SIZE,  PARTICLE_SIZE, 0.0f), D3DXVECTOR2(0.0f, 0.0) },
	{ D3DXVECTOR3(PARTICLE_SIZE,  PARTICLE_SIZE, 0.0f), D3DXVECTOR2(1.0f, 0.0) },
	{ D3DXVECTOR3(-PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f), D3DXVECTOR2(0.0f, 1.0) },
	{ D3DXVECTOR3(PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f), D3DXVECTOR2(1.0f, 1.0) },
};

//�e�풸�_�o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;		//�P�ʒ��_�p
static LPDIRECT3DVERTEXBUFFER9 worldBuff;	//���[���h�ϊ��p
static LPDIRECT3DVERTEXBUFFER9 uvBuff;		//�ʂ̃e�N�X�`��

//���_�錾
static LPDIRECT3DVERTEXDECLARATION9 declare;

//�V�F�[�_
static LPD3DXEFFECT effect;

//�C���f�b�N�X�o�b�t�@
static LPDIRECT3DINDEXBUFFER9 indexBuff;

//�e�N�X�`��
static LPDIRECT3DTEXTURE9 texture;

//�ŏI�I�ȕ`��p�̒��_�o�b�t�@�i�C���X�^���V���O�ɂ͊֌W�Ȃ��j
static LPDIRECT3DVERTEXBUFFER9 screenBuff;

//�p�[�e�B�N���̈ړ��X�s�[�h
static float speed[PARTICLE_NUM];

//�`��p�e�N�X�`���@�T�[�t�F�C�X
static LPDIRECT3DTEXTURE9 renderTexture;
static LPDIRECT3DSURFACE9 renderSurface;

//�`��N���X
ScreenObject *renderer;

/************************************
����������
*************************************/
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���ǂݍ��݂��Đݒ�
	texture = CreateTextureFromFile((LPSTR)"data/TEXTURE/bulletParticle.png", pDevice);
	pDevice->SetTexture(0, texture);

	//���_�o�b�t�@�쐬
	pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &vtxBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(Transform) * PARTICLE_NUM, 0, 0, D3DPOOL_MANAGED, &worldBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(ParticleTex) * PARTICLE_NUM, 0, 0, D3DPOOL_MANAGED, &uvBuff, 0);
	
	//�P�ʒ��_�̒��g�𖄂߂�
	ParticleVertex *pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	memcpy(pVtx, vtx, sizeof(vtx));
	vtxBuff->Unlock();

	//���_�o�b�t�@�i���[���h�ϊ��p�j�̒��g�𖄂߂�
	Transform *pTr;
	worldBuff->Lock(0, 0, (void**)&pTr, 0);
	for (int i = 0; i < PARTICLE_NUM; i++, pTr++)
	{
		pTr->pos = D3DXVECTOR3(RandomRangef(-PARTICLE_POS_X_MAX, PARTICLE_POS_X_MAX), RandomRangef(-PARTICLE_POS_Y_MAX, PARTICLE_POS_Y_MAX), RandomRangef(-500.0f, PARTICLE_POS_Z_MAX));
		pTr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pTr->scl = D3DXVECTOR3(0.5f, 0.5f, 1.0f);

		speed[i] = RandomRangef(PARTICLE_MOVE_SPEED * 0.8f, PARTICLE_MOVE_SPEED * 1.5f);
	}
	worldBuff->Unlock();

	//���_�o�b�t�@�i�e�N�X�`���p�j�̒��g�𖄂߂�
	ParticleTex *pTex;
	uvBuff->Lock(0, 0, (void**)&pTex, 0);
	for (int i = 0; i < PARTICLE_NUM; i++, pTex++)
	{
		pTex->tex = D3DXVECTOR2(0.0f, 0.0f);
	}
	uvBuff->Unlock();

	//���_�錾���쐬
	D3DVERTEXELEMENT9 elems[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//�P�ʒ��_�i���_���W�j
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	//�P�ʒ��_�i�e�N�X�`�����W�j
	{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	//���[���h�ϊ����i�|�W�V�����j
	{ 1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	//���[���h�ϊ����i���[�e�[�V�����j
	{ 1, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	//���[���h�ϊ����i�X�P�[���j
	{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	//�ʂ̃e�N�X�`��
	D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration(elems, &declare);

	//�C���f�b�N�X�o�b�t�@�쐬
	WORD index[6] = { 0, 1, 2, 2, 1, 3 };
	pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, NULL);
	void *p;
	//���g�𖄂߂�
	indexBuff->Lock(0, 0, &p, 0);
	memcpy(p, index, sizeof(index));
	indexBuff->Unlock();

	//�V�F�[�_�ǂݍ���
	D3DXCreateEffectFromFile(pDevice, "data/EFFECT/particle.fx", 0, 0, 0, 0, &effect, 0);

	//�e�N�X�`���쐬�@�T�[�t�F�C�X�擾
	pDevice->CreateTexture(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&renderTexture, 
		0);
	renderTexture->GetSurfaceLevel(0, &renderSurface);

	//�`��I�u�W�F�N�g�쐬
	renderer = new ScreenObject();
	renderer->Resize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
}

/************************************
�I������
*************************************/
void UninitParticle(void)
{
	SAFE_RELEASE(vtxBuff);
	SAFE_RELEASE(worldBuff);
	SAFE_RELEASE(uvBuff);
	SAFE_RELEASE(declare);
	SAFE_RELEASE(effect);

	SAFE_RELEASE(renderSurface);
	SAFE_RELEASE(renderTexture);

	delete renderer;
}

/************************************
�X�V����
*************************************/
void UpdateParticle(void)
{
	Transform *tr;
	worldBuff->Lock(0, 0, (void**)&tr, 0);

	for (int i = 0; i < PARTICLE_NUM; i++, tr++)
	{
		tr->pos.y -= speed[i];

		if (tr->pos.y < -PARTICLE_POS_Y_MAX)
			tr->pos.y = PARTICLE_POS_Y_MAX;
	}

	worldBuff->Unlock();
}

/************************************
�`�揈��
*************************************/
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	DrawColorAndBloom();

	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);

	pDevice->SetRenderTarget(0, renderSurface);
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

	//�C���X�^���V���O�`��
	//if (useEffect)
	{
		DrawColorAndBloom();	//��������ԏd�v
	}

	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	CrossFilterController::Instance()->Draw(renderTexture);


	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/*****************************************************
�C���X�^���V���O�������`�揈��
******************************************************/
void DrawColorAndBloom()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�X�g���[���\�[�X���g����ݒ�
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | PARTICLE_NUM);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//���_�錾�ݒ�
	pDevice->SetVertexDeclaration(declare);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, texture);

	//�X�g���[���\�[�X�ݒ�
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(ParticleVertex));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(Transform));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(ParticleTex));
	pDevice->SetIndices(indexBuff);

	//�r���[�s��A�v���W�F�N�V�����s��A�r���[�t�s��
	D3DXMATRIX  view, proj, invView;

	//�r���[�A�v���W�F�N�V�����s����擾
	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	//�r���{�[�h�p�ɋt�s����v�Z
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = invView._42 = invView._43 = 0.0f;

	//�V�F�[�_�ɍs���ݒ�
	effect->SetMatrix("mtxView", &view);
	effect->SetMatrix("mtxProj", &proj);
	effect->SetMatrix("mtxInvView", &invView);

	//�V�F�[�_�ɂ��`��J�n
	effect->SetTechnique("tech");
	effect->Begin(0, 0);
	effect->BeginPass(0);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
	effect->End();

	//�`�悪�I������̂ŃX�g���[���\�[�X�����ɖ߂�
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
}
