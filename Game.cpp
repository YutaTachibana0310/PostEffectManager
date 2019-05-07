//=====================================
//
//�Q�[������[Game.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "input.h"
#include "light.h"
#include "camera.h"
#include "debugWindow.h"
#include "debugTimer.h"
#include "ModelController.h"
#include "PostEffectManager.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
void CreateScreenVertexBuffer();
void CreateRenderTarget();

/**************************************
�O���[�o���ϐ�
***************************************/
//�|�X�g�G�t�F�N�g�}�l�[�W���C���X�^���X
PostEffectManager* PostEffectManager::instance = NULL;

//�ύX��̃r���[�|�[�g
static D3DVIEWPORT9 viewPort;

//�`��p�e�N�X�`��&�T�[�t�F�C�X
static LPDIRECT3DTEXTURE9 renderTexture;
static LPDIRECT3DSURFACE9 renderSurface;

//Z�}�b�v�p�e�N�X�`��&�T�[�t�F�C�X
static LPDIRECT3DTEXTURE9 zMapTexture;
static LPDIRECT3DSURFACE9 zMapSurface;

//�o�b�N�o�b�t�@�֕`�悷�邽�߂̒��_�o�b�t�@
static LPDIRECT3DVERTEXBUFFER9 screenVtx;

/**************************************
����������
***************************************/
void InitGame(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	CreateScreenVertexBuffer();
	CreateRenderTarget();

	InitInput(hInstance, hWnd);
	InitCamera();
	InitLight();
	InitDebugWindow(hWnd, pDevice);
	InitModelController(0);

	RegisterDebugTimer("Main");
}

/**************************************
�I������
***************************************/
void UninitGame()
{
	UninitInput();
	UninitLight();
	UninitDebugWindow(0);
	UninitDebugTimer();
	UninitModelController(0);

	PostEffectManager::Destroy();
}

/**************************************
�X�V����
***************************************/
void UpdateGame()
{
	UpdateDebugWindow();
	UpdateInput();
	UpdateLight();
	UpdateCamera();

	CountDebugTimer("Main", "UpdateModel");
	UpdateModelController();
	CountDebugTimer("Main", "UpdateModel");

	CountDebugTimer("Main", "UpdatePostEffect");
	PostEffectManager::Instance()->Update();
	CountDebugTimer("Main", "UpdatePostEffect");
}

/**************************************
�`�揈��
***************************************/
void DrawGame()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DVIEWPORT9 oldVirwPort;
	pDevice->GetViewport(&oldVirwPort);

	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);
	pDevice->SetRenderTarget(0, renderSurface);
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0, 1.0f, 0);

	SetCamera();

	CountDebugTimer("Main", "DrawModel");
	DrawModelController();
	CountDebugTimer("Main", "DrawModel");

	CountDebugTimer("Main", "DrawPostEffect");
	PostEffectManager::Instance()->Draw();
	CountDebugTimer("Main", "DrawPostEffect");

	CountDebugTimer("Main", "DrawBackBuffer");
	pDevice->SetViewport(&oldVirwPort);
	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	BeginDebugWindow("Game");
	DebugDrawTexture(renderTexture, 300, 150);
	EndDebugWindow("Game");


	pDevice->SetTexture(0, renderTexture);
	pDevice->SetStreamSource(0, screenVtx, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	CountDebugTimer("Main", "DrawBackBuffer");

	DrawDebugTimer("Main");

	DrawDebugWindow();
}

/**************************************
�`��p���_�o�b�t�@�쐬
***************************************/
void CreateScreenVertexBuffer()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&screenVtx,
		0);

	VERTEX_2D *pVtx;
	screenVtx->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	screenVtx->Unlock();
}

/**************************************
�����_�[�^�[�Q�b�g�쐬
***************************************/
void CreateRenderTarget()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�����_�[�e�N�X�`���쐬
	pDevice->CreateTexture(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&renderTexture,
		0);

	renderTexture->GetSurfaceLevel(0, &renderSurface);

	//Z�o�b�t�@�쐬

	//�r���[�|�[�g�쐬
	viewPort.Height = 2048;
	viewPort.Width = 2048;
	viewPort.MinZ = 0.0f;
	viewPort.MaxZ = 1.0f;
	viewPort.X = 0;
	viewPort.Y = 0;
}

LPDIRECT3DTEXTURE9 GetDrawDataTemp()
{
	return renderTexture;
}