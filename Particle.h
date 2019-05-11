#include "main.h"

/************************
パーティクルの単位頂点
**************************/
typedef struct
{
	D3DXVECTOR3 vtx;
	D3DXVECTOR2 tex;
}ParticleVertex;

/************************
パーティクルのテクスチャ
**************************/
typedef struct
{
	D3DXVECTOR2 tex;
}ParticleTex;

/************************
ワールド変換情報
**************************/
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scl;
}Transform;

/************************
プロトタイプ宣言
**************************/
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);