//=====================================
//
//�e���v���[�g�w�b�_[debugTimer.h]
//Author:GP11A341 21 ���ԗY��
//
//=====================================
#ifndef _DEBUGTIMER_H_
#define _DEBUGTIMER_H_

#include "main.h"

/**************************************
�}�N����`
***************************************/

/**************************************
�\���̒�`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
void UninitDebugTimer(void);
void DrawDebugTimer(const char* label);
bool RegisterDebugTimer(const char* label);
bool CountDebugTimer(const char* label, const char* tag);

#endif