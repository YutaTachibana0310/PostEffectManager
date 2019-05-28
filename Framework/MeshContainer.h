//=====================================
//
//メッシュコンテナヘッダ[MeshContainer.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _MESHCONTAINER_H_
#define _MESHCONTAINER_H_

#include "../main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/
class MeshContainer
{
public:
	MeshContainer();
	~MeshContainer();

	HRESULT Load(LPSTR filePath);
	void Draw();

private:
	LPD3DXMESH mesh;
	D3DMATERIAL9* materials;
	LPDIRECT3DTEXTURE9 *textures;
	DWORD materialNum;
};

#endif