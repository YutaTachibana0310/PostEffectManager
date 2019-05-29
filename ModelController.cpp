//=====================================
//
//���f���R���g���[������[ModelController.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "ModelController.h"
#include "Framework\MeshContainer.h"

/**************************************
�}�N����`
***************************************/
#define MODEL_KIND_MAX	(3)
#define MODEL_NUM_MAX	(10)

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/
MeshContainer *container[MODEL_KIND_MAX];


static const char* modelName[MODEL_KIND_MAX] =
{
	"data/MODEL/cube00.x",
	"data/MODEL/sphere00.x",
	//"data/MODEL/cube01.x",
	"data/MODEL/player001.x",
};

static D3DXVECTOR3 position[MODEL_KIND_MAX][MODEL_NUM_MAX]; 

static D3DXVECTOR3 rotation[MODEL_KIND_MAX] = {
	D3DXVECTOR3(0.2f, 0.8f, 0.0f),
	D3DXVECTOR3(0.0f, 0.8f, 0.0f),
	D3DXVECTOR3(0.0f, 0.0f, 0.0f)
};

/**************************************
�v���g�^�C�v�錾
***************************************/

/**************************************
����������
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
	for (int i = 0; i < MODEL_KIND_MAX; i++)
	{
		for (int j = 0; j < MODEL_NUM_MAX; j++)
		{
			position[i][j].x = (i - 1) * offset;
			position[i][j].y = -30.0f;
			position[i][j].z = j * offset;
		}
	}
}

/**************************************
�I������
***************************************/
void UninitModelController(int num)
{
	for (int i = 0; i < 3; i++)
	{
		delete container[i];
	}
}

/**************************************
�X�V����
***************************************/
void UpdateModelController(void)
{

}

/**************************************
�`�揈��
***************************************/
void DrawModelController(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxWorld, mtxTranslate, mtxRot;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < MODEL_KIND_MAX; i++)
	{
		for (int k = 0; k < MODEL_NUM_MAX; k++)
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