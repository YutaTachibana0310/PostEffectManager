#include "Particle.h"
#include "debugWindow.h"
#include "PostEffect\CrossFilterController.h"
#include "PostEffect\ScreenObject.h"

/************************************
マクロ定義
*************************************/
#define PARTICLE_NUM			(2048)
#define PARTICLE_POS_X_MAX		(500.0f)
#define PARTICLE_POS_Y_MAX		(200.0f)
#define PARTICLE_POS_Z_MAX		(2000.0f)
#define PARTICLE_MOVE_SPEED		(0.5f)
#define PARTICLE_SIZE			(5.0f)

/************************************
プロトタイプ宣言
*************************************/
void DrawColorAndBloom();

/************************************
グローバル変数
*************************************/
static Transform trasnform;
static ParticleTex uv;

//パーティクルの単位頂点
static ParticleVertex vtx[4] =
{
	{ D3DXVECTOR3(-PARTICLE_SIZE,  PARTICLE_SIZE, 0.0f), D3DXVECTOR2(0.0f, 0.0) },
	{ D3DXVECTOR3(PARTICLE_SIZE,  PARTICLE_SIZE, 0.0f), D3DXVECTOR2(1.0f, 0.0) },
	{ D3DXVECTOR3(-PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f), D3DXVECTOR2(0.0f, 1.0) },
	{ D3DXVECTOR3(PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f), D3DXVECTOR2(1.0f, 1.0) },
};

//各種頂点バッファ
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;		//単位頂点用
static LPDIRECT3DVERTEXBUFFER9 worldBuff;	//ワールド変換用
static LPDIRECT3DVERTEXBUFFER9 uvBuff;		//個別のテクスチャ

//頂点宣言
static LPDIRECT3DVERTEXDECLARATION9 declare;

//シェーダ
static LPD3DXEFFECT effect;

//インデックスバッファ
static LPDIRECT3DINDEXBUFFER9 indexBuff;

//テクスチャ
static LPDIRECT3DTEXTURE9 texture;

//最終的な描画用の頂点バッファ（インスタンシングには関係ない）
static LPDIRECT3DVERTEXBUFFER9 screenBuff;

//パーティクルの移動スピード
static float speed[PARTICLE_NUM];

//描画用テクスチャ　サーフェイス
static LPDIRECT3DTEXTURE9 renderTexture;
static LPDIRECT3DSURFACE9 renderSurface;

//描画クラス
ScreenObject *renderer;

/************************************
初期化処理
*************************************/
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ読み込みして設定
	texture = CreateTextureFromFile((LPSTR)"data/TEXTURE/bulletParticle.png", pDevice);
	pDevice->SetTexture(0, texture);

	//頂点バッファ作成
	pDevice->CreateVertexBuffer(sizeof(vtx), 0, 0, D3DPOOL_MANAGED, &vtxBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(Transform) * PARTICLE_NUM, 0, 0, D3DPOOL_MANAGED, &worldBuff, 0);
	pDevice->CreateVertexBuffer(sizeof(ParticleTex) * PARTICLE_NUM, 0, 0, D3DPOOL_MANAGED, &uvBuff, 0);

	//単位頂点の中身を埋める
	ParticleVertex *pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	memcpy(pVtx, vtx, sizeof(vtx));
	vtxBuff->Unlock();

	//頂点バッファ（ワールド変換用）の中身を埋める
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

	//頂点バッファ（テクスチャ用）の中身を埋める
	ParticleTex *pTex;
	uvBuff->Lock(0, 0, (void**)&pTex, 0);
	for (int i = 0; i < PARTICLE_NUM; i++, pTex++)
	{
		pTex->tex = D3DXVECTOR2(0.0f, 0.0f);
	}
	uvBuff->Unlock();

	//頂点宣言を作成
	D3DVERTEXELEMENT9 elems[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//単位頂点（頂点座標）
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },	//単位頂点（テクスチャ座標）
	{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },	//ワールド変換情報（ポジション）
	{ 1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },	//ワールド変換情報（ローテーション）
	{ 1, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },	//ワールド変換情報（スケール）
	{ 2, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },	//個別のテクスチャ
	D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration(elems, &declare);

	//インデックスバッファ作成
	WORD index[6] = { 0, 1, 2, 2, 1, 3 };
	pDevice->CreateIndexBuffer(sizeof(index), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &indexBuff, NULL);
	void *p;
	//中身を埋める
	indexBuff->Lock(0, 0, &p, 0);
	memcpy(p, index, sizeof(index));
	indexBuff->Unlock();

	//シェーダ読み込み
	D3DXCreateEffectFromFile(pDevice, "data/EFFECT/particle.fx", 0, 0, 0, 0, &effect, 0);

	//テクスチャ作成　サーフェイス取得
	pDevice->CreateTexture(SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&renderTexture,
		0);
	renderTexture->GetSurfaceLevel(0, &renderSurface);

	//描画オブジェクト作成
	renderer = new ScreenObject();
	renderer->Resize((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
}

/************************************
終了処理
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
更新処理
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
描画処理
*************************************/
void DrawParticle(void)
{
	BeginDebugWindow("PostEffect");
	static bool useCrossFilter = true;
	DebugChechBox("Use CrossFilter", &useCrossFilter);
	EndDebugWindow("PostEffect");

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	DrawColorAndBloom();

	if (useCrossFilter)
	{
		LPDIRECT3DSURFACE9 oldSuf;
		pDevice->GetRenderTarget(0, &oldSuf);

		pDevice->SetRenderTarget(0, renderSurface);
		pDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 0, 0);

		DrawColorAndBloom();	//ここが一番重要

		pDevice->SetRenderTarget(0, oldSuf);
		SAFE_RELEASE(oldSuf);

		CrossFilterController::Instance()->Draw(renderTexture);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/*****************************************************
インスタンシングをした描画処理
******************************************************/
void DrawColorAndBloom()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ストリームソース周波数を設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | PARTICLE_NUM);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点宣言設定
	pDevice->SetVertexDeclaration(declare);

	//テクスチャ設定
	pDevice->SetTexture(0, texture);

	//ストリームソース設定
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(ParticleVertex));
	pDevice->SetStreamSource(1, worldBuff, 0, sizeof(Transform));
	pDevice->SetStreamSource(2, uvBuff, 0, sizeof(ParticleTex));
	pDevice->SetIndices(indexBuff);

	//ビュー行列、プロジェクション行列、ビュー逆行列
	D3DXMATRIX  view, proj, invView;

	//ビュー、プロジェクション行列を取得
	pDevice->GetTransform(D3DTS_VIEW, &view);
	pDevice->GetTransform(D3DTS_PROJECTION, &proj);

	//ビルボード用に逆行列を計算
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = invView._42 = invView._43 = 0.0f;

	//シェーダに行列を設定
	effect->SetMatrix("mtxView", &view);
	effect->SetMatrix("mtxProj", &proj);
	effect->SetMatrix("mtxInvView", &invView);

	//シェーダによる描画開始
	effect->SetTechnique("tech");
	effect->Begin(0, 0);
	effect->BeginPass(0);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	effect->EndPass();
	effect->End();

	//描画が終わったのでストリームソースを元に戻す
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
}
