//=====================================
//
//テンプレートヘッダ[debugTimer.h]
//Author:GP12B332 21 立花雄太
//
//=====================================
#ifndef _DEBUGTIMER_H_
#define _DEBUGTIMER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void UninitDebugTimer(void);
void DrawDebugTimer(const char* label);
bool RegisterDebugTimer(const char* label);
bool CountDebugTimer(const char* label, const char* tag);

#endif