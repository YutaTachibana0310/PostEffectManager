//=====================================
//
//�Q�[���w�b�_[Game.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�N���X��`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
void InitGame(HINSTANCE hInstance, HWND hWnd);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

LPDIRECT3DTEXTURE9 GetDrawDataTemp();
#endif