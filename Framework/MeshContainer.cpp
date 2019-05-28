//=====================================
//
//���b�V���R���e�i����[MeshContainer.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "MeshContainer.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/

/**************************************
�R���X�g���N�^
***************************************/
MeshContainer::MeshContainer()
{

}

/**************************************
�f�X�g���N�^
***************************************/
MeshContainer::~MeshContainer()
{
	SAFE_RELEASE(mesh);
}

/**************************************
�t�@�C���ǂݍ��ݏ���
***************************************/
HRESULT MeshContainer::Load(LPSTR filePath)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	LPD3DXBUFFER tmpMaterial;

	//X�t�@�C���ǂݍ���
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

	//�}�e���A����D3DXMATERIAL�Ƃ��ĕ���
	materials = (D3DMATERIAL9*)malloc(sizeof(D3DMATERIAL9) * materialNum);
	D3DXMATERIAL* matBuffer = (D3DXMATERIAL*)tmpMaterial->GetBufferPointer();
	for (DWORD i = 0; i < materialNum; i++)
	{
		materials[i] = matBuffer[i].MatD3D;
		//materials[i].Ambient = materials[i].Diffuse;		//���ꂢ��H
	}

	//�e�N�X�`���ǂݍ���

	return res;
}

/**************************************
�`�揈��
***************************************/
void MeshContainer::Draw()
{
	D3DMATERIAL9 matDef;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�}�e���A���ޔ�
	pDevice->GetMaterial(&matDef);

	for (DWORD i = 0; i < materialNum; i++)
	{
		//�e�N�X�`���̐ݒ�

		//�}�e���A���ݒ�
		pDevice->SetMaterial(&materials[i]);

		//�`��
		mesh->DrawSubset(i);
	}

	//�}�e���A������
	pDevice->SetMaterial(&matDef);
}