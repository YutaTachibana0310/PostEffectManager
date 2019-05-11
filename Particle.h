#include "main.h"

/************************
�p�[�e�B�N���̒P�ʒ��_
**************************/
typedef struct
{
	D3DXVECTOR3 vtx;
	D3DXVECTOR2 tex;
}ParticleVertex;

/************************
�p�[�e�B�N���̃e�N�X�`��
**************************/
typedef struct
{
	D3DXVECTOR2 tex;
}ParticleTex;

/************************
���[���h�ϊ����
**************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
}Transform;

/************************
�v���g�^�C�v�錾
**************************/
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);