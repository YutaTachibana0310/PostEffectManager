//=====================================
//
//モデルコントローラ処理[ModelController.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "ModelController.h"
#include "Framework\MeshContainer.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
MeshContainer *container[3];


static const char* modelName[3] =
{
	"data/MODEL/cube00.x",
	"data/MODEL/sphere00.x",
	"data/MODEL/cube01.x",
};

static D3DXVECTOR3 position[3][50];

static D3DXVECTOR3 rotation[3] = {
	D3DXVECTOR3(0.2f, 0.8f, 0.0f),
	D3DXVECTOR3(0.0f, 0.8f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitModelController(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < 3; i++)
	{
		container[i] = new MeshContainer();
		container[i]->Load((LPSTR)modelName[i]);
	}

	const float offset = 100.0f;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			position[i][j].x = (i - 1) * offset;
			position[i][j].y = -30.0f;
			position[i][j].z = j * offset;
		}
	}
}

/**************************************
終了処理
***************************************/
void UninitModelController(int num)
{
	for (int i = 0; i < 3; i++)
	{
		delete container[i];
	}
}

/**************************************
更新処理
***************************************/
void UpdateModelController(void)
{

}

/**************************************
描画処理
***************************************/
void DrawModelController(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxWorld, mtxTranslate, mtxRot;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 50; k++)
		{
			D3DXMatrixIdentity(&mtxWorld);

			D3DXMatrixRotationYawPitchRoll(&mtxRot, rotation[i].y, rotation[i].x, rotation[i].z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

			D3DXMatrixTranslation(&mtxTranslate, position[i][k].x, position[i][k].y, position[i][k].z);
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			container[i]->Draw();
		}
	}

	pDevice->SetMaterial(&matDef);
}