//=====================================
//
//�f�o�b�O�E�B���h�E�w�b�_[debugWindow.h]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#ifndef _DEBUGWINDOW_H_
#define _DEBUGWINDOW_H_

#include "main.h"

#include <string>

/**************************************
�}�N����`
***************************************/
#define USE_DEBUGWINDOW
#define STR(var) #var		//var�𕶎���ɒu������}�N��
/**************************************
�\���̒�`
***************************************/

/**************************************
�v���g�^�C�v�錾
***************************************/
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitDebugWindow(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
void UninitDebugWindow(int num);
void UpdateDebugWindow(void);
void DrawDebugWindow(void);

void SetActiveDebugWindow(bool state = true);	//�\���ؑ֏���

void BeginTimerCount(void);				//���𑜓x�^�C�}�[�v���J�n
double GetProgressTimerCount(void);		//�^�C�}�[�o�ߎ��Ԏ擾����
void GetTimerCount(LARGE_INTEGER *ptr);	//�^�C�}�[�J�E���g�擾(20�t���[���Ԋu)
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end);	//�o�ߎ��Ԏ擾	

//ImGUI���b�p�[�֐�
void BeginDebugWindow(const char *label);
void EndDebugWindow(const char* label);
void DebugText(const char *str, ...);
void DebugText(std::string str);
bool DebugButton(const char *label);
bool DebugButton(const std::string *label);
bool DebugSliderFloat(const char *label, float *adr, float min, float max);
bool DebugSliderFloat(const std::string *label, float *adr, float min, float max);
void DebugColorEditor(const char *label, float array[4]);
void DebugNewLine(void);
void DebugSameLine(void);
void DebugTreeExpansion(bool isOpen);
bool DebugTreePush(const char *label);
void DebugTreePop(void);
void DebugProgressBar(float fraction, const char* label, D3DXVECTOR2 size = D3DXVECTOR2(-1.0f, 0.0f));
void DebugInputVector3(const char* label, D3DXVECTOR3 *vec);
void DebugSliderVector3(const char* label, D3DXVECTOR3 *vec, float min, float max);
void DebugInputVector2(const char* label, D3DXVECTOR2 *vec);
void DebugInputFloat(const char* label, float *var);
bool DebugInputInt(const char* label, int* val);
bool DebugInputText(const char* label, char *buf, size_t buf_size);
bool DebugInputText(const char* label, std::string* pStr);
bool DebugRadioButton(const char* label, int* output, int val);
bool DebugChechBox(const char* label, bool* val);
void DebugDrawTexture(LPDIRECT3DTEXTURE9 texture, float sizeX, float sizeY);
#endif