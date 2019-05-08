//=====================================
//
//ゲーム処理[Game.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "input.h"
#include "light.h"
#include "camera.h"
#include "debugWindow.h"
#include "debugTimer.h"
#include "ModelController.h"
#include "PostEffectManager.h"
#include "Particle.h"

/**************************************
マクロ定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void CreateScreenVertexBuffer();
void CreateRenderTarget();

/**************************************
グローバル変数
***************************************/
//ポストエフェクトマネージャインスタンス
PostEffectManager* PostEffectManager::instance = NULL;

//変更後のビューポート
static D3DVIEWPORT9 viewPort;

//描画用テクスチャ&サーフェイス
static LPDIRECT3DTEXTURE9 renderTexture;
static LPDIRECT3DSURFACE9 renderSurface;

//Zマップ用テクスチャ&サーフェイス
static LPDIRECT3DTEXTURE9 zMapTexture;
static LPDIRECT3DSURFACE9 zMapSurface;

//バックバッファへ描画するための頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 screenVtx;

/**************************************
初期化処理
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
	InitParticle();

	RegisterDebugTimer("Main");
}

/**************************************
終了処理
***************************************/
void UninitGame()
{
	UninitInput();
	UninitLight();
	UninitDebugWindow(0);
	UninitDebugTimer();
	UninitModelController(0);
	UninitParticle();

	PostEffectManager::Destroy();
}

/**************************************
更新処理
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

	CountDebugTimer("Main", "UpdateParticle");
	UpdateParticle();
	CountDebugTimer("Main", "UpdateParticle");

	CountDebugTimer("Main", "UpdatePostEffect");
	PostEffectManager::Instance()->Update();
	CountDebugTimer("Main", "UpdatePostEffect");
}

/**************************************
描画処理
***************************************/
void DrawGame()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//現在のビューポートを退避してレンダーターゲットを切り替え
	D3DVIEWPORT9 oldVirwPort;
	pDevice->GetViewport(&oldVirwPort);

	//バックバッファを退避してレンダーターゲットを切り替え
	LPDIRECT3DSURFACE9 oldSuf;
	pDevice->GetRenderTarget(0, &oldSuf);
	pDevice->SetRenderTarget(0, renderSurface);
	pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0, 1.0f, 0);

	//オブジェクトを描画
	SetCamera();

	CountDebugTimer("Main", "DrawModel");
	DrawModelController();
	CountDebugTimer("Main", "DrawModel");

	CountDebugTimer("Main", "DrawParticle");
	DrawParticle();
	CountDebugTimer("Main", "DrawParticle");

	//ポストエフェクト反映
	CountDebugTimer("Main", "DrawPostEffect");
	PostEffectManager::Instance()->Draw();
	CountDebugTimer("Main", "DrawPostEffect");

	//結果をバックバッファへと描画
	CountDebugTimer("Main", "DrawBackBuffer");
	pDevice->SetViewport(&oldVirwPort);
	pDevice->SetRenderTarget(0, oldSuf);
	SAFE_RELEASE(oldSuf);

	pDevice->SetTexture(0, renderTexture);
	pDevice->SetStreamSource(0, screenVtx, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	CountDebugTimer("Main", "DrawBackBuffer");

	DrawDebugTimer("Main");

	DrawDebugWindow();
}

/**************************************
描画用頂点バッファ作成
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
レンダーターゲット作成
***************************************/
void CreateRenderTarget()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//レンダーテクスチャ作成
	pDevice->CreateTexture(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&renderTexture,
		0);

	renderTexture->GetSurfaceLevel(0, &renderSurface);

	//Zバッファ作成

	//ビューポート作成
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