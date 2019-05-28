//=====================================
//
//メッシュコンテナ処理[MeshContainer.cpp]
//Author:GP12B332 21 立花雄太
//
//=====================================
#include "MeshContainer.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
コンストラクタ
***************************************/
MeshContainer::MeshContainer()
{

}

/**************************************
デストラクタ
***************************************/
MeshContainer::~MeshContainer()
{
	SAFE_RELEASE(mesh);
}

/**************************************
ファイル読み込み処理
***************************************/
HRESULT MeshContainer::Load(LPSTR filePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXBUFFER tmpMaterial;

	//Xファイル読み込み
	HRESULT res = D3DXLoadMeshFromX(filePath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&tmpMaterial,
		NULL,
		&materialNum,
		&mesh);

	if (res != S_OK)
	{
		return res;
	}

	//マテリアルをD3DXMATERIALとして複写
	materials = (D3DMATERIAL9*)malloc(sizeof(D3DMATERIAL9) * materialNum);
	D3DXMATERIAL* matBuffer = (D3DXMATERIAL*)tmpMaterial->GetBufferPointer();
	for (DWORD i = 0; i < materialNum; i++)
	{
		materials[i] = matBuffer[i].MatD3D;
		//materials[i].Ambient = materials[i].Diffuse;		//これいる？
	}

	//テクスチャ読み込み

	return res;
}

/**************************************
描画処理
***************************************/
void MeshContainer::Draw()
{
	D3DMATERIAL9 matDef;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//マテリアル退避
	pDevice->GetMaterial(&matDef);

	for (DWORD i = 0; i < materialNum; i++)
	{
		//テクスチャの設定

		//マテリアル設定
		pDevice->SetMaterial(&materials[i]);

		//描画
		mesh->DrawSubset(i);
	}

	//マテリアル復元
	pDevice->SetMaterial(&matDef);
}