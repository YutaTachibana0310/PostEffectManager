//=====================================
//
//ゲームヘッダ[Game.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
クラス定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void InitGame(HINSTANCE hInstance, HWND hWnd);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

LPDIRECT3DTEXTURE9 GetDrawDataTemp();
#endif