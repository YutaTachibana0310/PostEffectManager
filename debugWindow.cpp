//=====================================
//
//�e���v���[�g����[template.cpp]
//Author:GP12B332 21 ���ԗY��
//
//=====================================
#include "debugWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "input.h"

/**************************************
�}�N����`
***************************************/
#define INTERBAL_GETTIMER		(20)
#define USE_DEBUGFUNC _DEBUG

/**************************************
�\���̒�`
***************************************/

/**************************************
�O���[�o���ϐ�
***************************************/
LARGE_INTEGER timeCountBegin;
static unsigned int cntFrame = 0;
static bool enableDraw = true;

/**************************************
�v���g�^�C�v�錾
***************************************/
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**************************************
�f�o�b�O�E�B���h�E�p�R�[���o�b�N
***************************************/
LRESULT DebugWindPrcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#ifdef USE_DEBUGFUNC
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
#else
	return S_OK;
#endif
}

/**************************************
����������
***************************************/
void InitDebugWindow(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
#ifdef USE_DEBUGFUNC
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);

	ImGui::StyleColorsDark();
#endif
}

/**************************************
�I������
***************************************/
void UninitDebugWindow(int num)
{
#ifdef USE_DEBUGFUNC
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
#endif
}

/**************************************
�X�V����
***************************************/
void UpdateDebugWindow(void)
{
#ifdef USE_DEBUGFUNC
	if (GetKeyboardTrigger(DIK_D) && GetKeyboardPress(DIK_LCONTROL))
	{
		enableDraw = !enableDraw;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX9_NewFrame();
	ImGui::NewFrame();

	cntFrame++;
#endif
}

/**************************************
�`�揈��
***************************************/
void DrawDebugWindow(void)
{
#ifdef USE_DEBUGFUNC
	if (!enableDraw)
	{
		ImGui::EndFrame();
		return;
	}
	
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
}

/**************************************
���𑜓x�^�C�}�[�ł̌v���J�n����
***************************************/
void BeginTimerCount(void)
{
#ifdef USE_DEBUGFUNC
	QueryPerformanceCounter(&timeCountBegin);
#endif
}

/**************************************
�v���J�n����̌o�ߎ���[�P�ʁFmsec]
***************************************/
double GetProgressTimerCount(void)
{
#ifdef USE_DEBUGFUNC
	//�^�C�}�[�̎��g�����擾
	LARGE_INTEGER frequencyTimer;
	QueryPerformanceFrequency(&frequencyTimer);

	//�J�E���g�擾
	LARGE_INTEGER timeCurrent;
	QueryPerformanceCounter(&timeCurrent);

	//�v���J�n����̌o�ߎ���[msec]���v�Z
	LONGLONG span = timeCurrent.QuadPart - timeCountBegin.QuadPart;
	double msec = (double)span * 1000 / (double)frequencyTimer.QuadPart;

	return msec;
#else 
	return 0.0f;
#endif
}

/**************************************
�^�C�}�[�J�E���g�擾�����i20�t���[�������ɓ���j
***************************************/
void GetTimerCount(LARGE_INTEGER *ptr)
{
#ifdef	USE_DEBUGFUNC
	if (cntFrame % INTERBAL_GETTIMER != 0)
		return;

	QueryPerformanceCounter(ptr);
#endif
}

/**************************************
�o�ߎ��Ԏ擾����[msec]
***************************************/
double CalcProgressTime(LARGE_INTEGER start, LARGE_INTEGER end)
{
#ifdef USE_DEBUGFUNC
	//�^�C�}�[�̎��g���擾
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	//�o�ߎ��Ԃ��v�Z
	LONGLONG span = end.QuadPart - start.QuadPart;
	double msec = (double)span * 1000.0f / (double)frequency.QuadPart;

	return msec;
#else
	return 0.0f;
#endif
}

/*************************************
�\���؂�ւ�����
***************************************/
void SetActiveDebugWindow(bool state)
{
	enableDraw = state;
}

/*************************************
�f�o�b�O�E�B���h�E�J�n����
***************************************/
void BeginDebugWindow(const char *label)
{
#ifdef USE_DEBUGFUNC
	ImGui::Begin(label);
#endif
}

/*************************************
�f�o�b�O�E�B���h�E�I������
***************************************/
void EndDebugWindow(const char *label)
{
#ifdef USE_DEBUGFUNC
	ImGui::End();
#endif
}

/*************************************
�f�o�b�O�e�L�X�g�\������
***************************************/
void DebugText(const char *str, ...)
{
#ifdef USE_DEBUGFUNC
	va_list ap;
	va_start(ap, str);
	ImGui::TextV(str, ap);
	//ImGui::Text(str, ap);
	va_end(ap);
#endif
}

/*************************************
�f�o�b�O�e�L�X�g�\������
***************************************/
void DebugText(std::string str)
{
#ifdef USE_DEBUGWINDOW
	DebugText(str.c_str());
#endif
}

/*************************************
�f�o�b�O�{�^���\������
***************************************/
bool DebugButton(const char *label)
{
#ifdef USE_DEBUGFUNC
	return ImGui::Button(label);
#else
	return false;
#endif
}

/*************************************
�f�o�b�O�{�^���\������(string��)
***************************************/
bool DebugButton(const std::string *label)
{
#ifdef USE_DEBUGFUNC
	return ImGui::Button(label->c_str());
#else
	return false;
#endif
}

/*************************************
�f�o�b�O�X���C�_�[����
***************************************/
bool DebugSliderFloat(const char *label, float *adr, float min, float max)
{
#ifdef USE_DEBUGFUNC
	return ImGui::SliderFloat(label, adr, min, max);

#else
	return false;
#endif
}

/*************************************
�f�o�b�O�X���C�_�[����(string��)
***************************************/
bool DebugSliderFloat(const std::string *label, float *adr, float min, float max)
{
#ifdef USE_DEBUGFUNC
	return ImGui::SliderFloat(label->c_str(), adr, min, max);
#else
	return false;
#endif
}

/*************************************
�f�o�b�O�J���[�s�b�J�[����
***************************************/
void DebugColorEditor(const char *label, float array[4])
{
#ifdef USE_DEBUGFUNC
	ImGui::ColorEdit4(label, array);
#endif
}

/*************************************
�f�o�b�O�E�B���h�E���s����
***************************************/
void DebugNewLine(void)
{
#ifdef USE_DEBUGFUNC
	ImGui::NewLine();
#endif
}

/*************************************
�f�o�b�O�E�B���h�E�A���s����
***************************************/
void DebugSameLine(void)
{
#ifdef USE_DEBUGFUNC
	ImGui::SameLine();
#endif
}

/*************************************
�c���[�\���W�J����
***************************************/
void DebugTreeExpansion(bool isOpen)
{
#ifdef USE_DEBUGFUNC
	ImGui::SetNextTreeNodeOpen(isOpen, ImGuiSetCond_Once);
#endif
}

/*************************************
�c���[�\���v�b�V������
***************************************/
bool DebugTreePush(const char *label)
{
#ifdef USE_DEBUGFUNC
	return ImGui::TreeNode(label);
#else
	return  false;
#endif
}

/*************************************
�c���[�\���|�b�v����
***************************************/
void DebugTreePop(void)
{
#ifdef USE_DEBUGFUNC
	ImGui::TreePop();
#endif
}

/*************************************
�v���O���X�o�[����
***************************************/
void DebugProgressBar(float fraction, const char* label, D3DXVECTOR2 size)
{
#ifdef USE_DEBUGFUNC
	ImGui::ProgressBar(fraction, ImVec2(size.x, size.y), label);
#endif
}

/*************************************
3�����x�N�g�����͏���
***************************************/
void DebugInputVector3(const char* label, D3DXVECTOR3 *vec)
{
#ifdef USE_DEBUGFUNC
	ImGui::InputFloat3(label, (float*)vec);
#endif
}

/*************************************
3�����x�N�g���X���C�_�[���͏���
***************************************/
void DebugSliderVector3(const char* label, D3DXVECTOR3 *vec, float min, float max)
{
#ifdef USE_DEBUGFUNC
	ImGui::SliderFloat3(label, (float*)vec, min, max);
#endif
}

/*************************************
2�����x�N�g�����͏���
***************************************/
void DebugInputVector2(const char* label, D3DXVECTOR2 *vec)
{
#ifdef USE_DEBUGFUNC
	ImGui::InputFloat2(label, (float*)vec);
#endif
}

/*************************************
float�^���͏���
***************************************/
void DebugInputFloat(const char* label, float *var)
{
#ifdef USE_DEBUGFUNC
	ImGui::InputFloat(label, var);
#endif
}

/*************************************
int�^���͏���
***************************************/
bool DebugInputInt(const char* label, int* val)
{
#ifdef USE_DEBUGWINDOW
	return ImGui::InputInt(label, val);
#else
	return false;
#endif
}

/*************************************
��������͏���
***************************************/
bool DebugInputText(const char* label, char *buf, size_t buf_size)
{
#ifdef USE_DEBUGFUNC
	return ImGui::InputText(label, buf, buf_size);
#else
	return false;
#endif
}

/*************************************
��������͏���
***************************************/
bool DebugInputText(const char* label, std::string* pStr)
{
#ifdef USE_DEBUGWINDOW
	char tmp[128];
	strcpy(tmp, pStr->c_str());
	bool res = ImGui::InputText(label, tmp, 128);
	if (res)
	{
		*pStr = tmp;
	}
	return res;
#else
	return false;
#endif
}

/*************************************
���W�I�{�^������
***************************************/
bool DebugRadioButton(const char* label, int* output, int val)
{
#ifdef USE_DEBUGWINDOW
	return ImGui::RadioButton(label, output, val);
#else
	return false;
#endif
}

/*************************************
�`�F�b�N�{�b�N�X����
***************************************/
bool DebugChechBox(const char* label, bool* val)
{
#ifdef USE_DEBUGWINDOW
	return ImGui::Checkbox(label, val);
#else
	return false;
#endif
}

/*************************************
�e�N�X�`���\������
***************************************/
void DebugDrawTexture(LPDIRECT3DTEXTURE9 texture, float sizeX, float sizeY)
{
#ifdef USE_DEBUGWINDOW
	ImGui::Image((void*)texture, ImVec2(sizeX, sizeY));
#endif
}