//=============================================================================
//
// ���͏��� [input.cpp]
// Author : 
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX		(256)			// �L�[�ő吔
// game pad�p�ݒ�l
#define DEADZONE		5000			// �e����25%�𖳌��]�[���Ƃ���
#define RANGE_MAX		1000			// �L���͈͂̍ő�l
#define RANGE_MIN		-1000			// �L���͈͂̍ŏ��l

#define INPUT_SHORTWAIT (6)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // �}�E�X�̏�����
void UninitMouse();						// �}�E�X�̏I������
HRESULT UpdateMouse();					// �}�E�X�̍X�V����

HRESULT InitializePad(void);			// �p�b�h������
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// �p�b�h�����R�[���o�b�N
void UpdatePad(void);
void UninitPad(void);

int GetPadAxisXPressed(int padNo);
int GetPadAxisXTriggered(int padNo);
int GetPadAxisYPressed(int padNo);
int GetPadAxisYTriggered(int padNo);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
BYTE					g_aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̉�����Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// �L�[�{�[�h�̃g���K�[��Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// �L�[�{�[�h�̃����[�X��Ԃ�ێ����郏�[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g��Ԃ�ێ����郏�[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// �}�E�X�̃_�C���N�g�ȏ��
static DIMOUSESTATE2   mouseTrigger;	// �����ꂽ�u�Ԃ���ON

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// �p�b�h�f�o�C�X

static DWORD	padState[GAMEPADMAX];	// �p�b�h���i�����Ή��j
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;			// ���o�����p�b�h�̐�

//�X�e�B�b�N��Repeat��Ԍ��o�p
static int		padAxisXRepeat[GAMEPADMAX];
static int		padAxisYRepeat[GAMEPADMAX];
static int		lastAxisX[GAMEPADMAX];
static int		lastAxisY[GAMEPADMAX];
static int		axisXRepeatCnt[GAMEPADMAX];
static int		axisYRepeatCnt[GAMEPADMAX];

static float	padAxislRx[GAMEPADMAX];
static float	padAxislRy[GAMEPADMAX];

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetHorizontalInputPress(int i)
{
#ifndef _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_LEFT) || IsButtonPressed(0, BUTTON_LEFT))
		{
			return -1;
		}

		if (GetKeyboardPress(DIK_RIGHT) || IsButtonPressed(0, BUTTON_RIGHT))
		{
			return 1;
		}
		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̑���̂�
	else
	{
		if (IsButtonPressed(i, BUTTON_LEFT))
			return -1;
		
		if (IsButtonPressed(i, BUTTON_RIGHT))
			return 1;

		return 0;
	}
#else
	//0�Ԃ̃v���C���[�̓L�[�{�[�h�ő���
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_LEFT))
			return -1;

		if (GetKeyboardPress(DIK_RIGHT))
			return 1;

		return 0;
	}
	//1�Ԉȍ~�̓p�b�h�ő���
	else
	{
		if (IsButtonPressed(i - 1, BUTTON_LEFT))
			return -1;

		if (IsButtonPressed(i - 1, BUTTON_RIGHT))
			return 1;

		return 0;
	}
#endif
}

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetVerticalInputPress(int i)
{
#ifndef _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_UP) || IsButtonPressed(0, BUTTON_UP))
		{
			return 1;
		}

		if (GetKeyboardPress(DIK_DOWN) || IsButtonPressed(0, BUTTON_DOWN))
		{
			return -1;
		}
		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̂ݑ���\
	else
	{
		if (IsButtonPressed(i, BUTTON_UP))
			return 1;
		
		if (IsButtonPressed(i, BUTTON_DOWN))
			return -1;

		return 0;
	}
#else
	//0�Ԃ̃v���C���[�̓L�[�{�[�h�ő���
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_DOWN))
			return -1;

		if (GetKeyboardPress(DIK_UP))
			return 1;

		return 0;
	}
	//1�Ԉȍ~�̓p�b�h�ő���
	else
	{
		if (IsButtonPressed(i - 1, BUTTON_DOWN))
			return -1;

		if (IsButtonPressed(i - 1, BUTTON_UP))
			return 1;

		return 0;
	}
#endif
}

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetHorizontalInputRepeat(int i)
{
#ifndef _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{
		if (padAxisXRepeat[0] != 0)
			return padAxisXRepeat[0];

		if (GetKeyboardRepeat(DIK_LEFT))
			return -1;

		if (GetKeyboardRepeat(DIK_RIGHT))
			return 1;

		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̂ݑ���\
	else
	{
		return padAxisXRepeat[i];
	}
#else
	//0�Ԃ̃v���C���[�̓L�[�{�[�h�ő���
	if (i == 0)
	{
		if (GetKeyboardRepeat(DIK_LEFT))
			return -1;

		if (GetKeyboardRepeat(DIK_RIGHT))
			return 1;

		return 0;
	}
	//1�Ԉȍ~�̓p�b�h�ő���
	else
	{
		return padAxisXRepeat[i - 1];
	}
#endif
}

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetVerticalInputRepeat(int i)
{
#ifndef _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{
		if (padAxisYRepeat[0] != 0)
			return padAxisYRepeat[0];

		if (GetKeyboardRepeat(DIK_UP))
			return 1;

		if (GetKeyboardRepeat(DIK_DOWN))
			return -1;

		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̂ݑ���\
	else
	{
		return padAxisYRepeat[i];
	}
#else
	if (i == 0)
	{
		if (GetKeyboardRepeat(DIK_UP))
			return 1;

		if (GetKeyboardRepeat(DIK_DOWN))
			return -1;

		return 0;
	}
	else
	{
		return padAxisYRepeat[i - 1];
	}
#endif
}

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetHorizontalInputTrigger(int i)
{
#ifndef  _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{

		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
			return -1;

		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
			return 1;

		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̂ݑ���\
	else
	{
		if (IsButtonTriggered(i, BUTTON_LEFT))
			return -1;

		if (IsButtonTriggered(i, BUTTON_RIGHT))
			return 1;

		return 0;		
	}
#else
	if (i == 0)
	{
		if (GetKeyboardTrigger(DIK_LEFT))
			return -1;

		if (GetKeyboardTrigger(DIK_RIGHT))
			return 1;

		return 0;
	}
	else
	{
		if (IsButtonTriggered(i - 1, BUTTON_LEFT))
			return -1;

		if (IsButtonTriggered(i - 1, BUTTON_RIGHT))
			return 1;

		return 0;
	}
#endif
}

//=============================================================================
// ���������̓��͂̎擾
//=============================================================================
int GetVerticalInputTrigger(int i)
{
#ifndef _DEBUG
	//0�Ԃ̃v���C���[�̂݃L�[�{�[�h�ł�����\
	if (i == 0)
	{
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
			return 1;

		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
			return -1;

		return 0;
	}
	//����ȊO�̓Q�[���p�b�h�ł̂ݑ���\
	else
	{
		if (IsButtonTriggered(i, BUTTON_UP))
			return 1;

		if (IsButtonTriggered(i, BUTTON_DOWN))
			return -1;

		return 0;
	}
#else
	if (i == 0)
	{
		if (GetKeyboardTrigger(DIK_UP))
			return 1;

		if (GetKeyboardTrigger(DIK_DOWN))
			return -1;

		return 0;
	}
	else
	{
		if (IsButtonTriggered(i - 1, BUTTON_UP))
			return 1;

		if (IsButtonTriggered(i - 1, BUTTON_DOWN))
			return 1;

		return 0;
	}
#endif
}

//=============================================================================
// �U���{�^�������̎擾
//=============================================================================
bool GetAttackButtonTrigger(int i)
{
#ifndef _DEBUG
	if (i == 0)
		return GetKeyboardTrigger(DIK_Z) || IsButtonTriggered(0, BUTTON_A);
	else
		return IsButtonTriggered(i, BUTTON_A);
#else
	if (i == 0)
		return GetKeyboardTrigger(DIK_Z);
	else
		return IsButtonTriggered(i - 1, BUTTON_A);
#endif
}

//=============================================================================
// �U���{�^���������ςȂ��̎擾
//=============================================================================
bool GetAttackButtonPress(int i)
{
#ifndef _DEBUG
	if (i == 0)
		return GetKeyboardPress(DIK_Z) || IsButtonPressed(0, BUTTON_A);
	else
		return IsButtonPressed(i, BUTTON_A);
#else
	if (i == 0)
		return GetKeyboardPress(DIK_Z);
	else
		return IsButtonPressed(i - 1, BUTTON_A);
#endif
}

//=============================================================================
// �U���{�^�������̎擾
//=============================================================================
bool GetAttackButtonRelease(int i)
{
#ifndef _DEBUG
	if (i == 0)
		return GetKeyboardRelease(DIK_Z) || IsButtonReleased(0, BUTTON_A);
	else
		return IsButtonReleased(i, BUTTON_A);
#else
	if (i == 0)
		return GetKeyboardRelease(DIK_Z);
	else
		return IsButtonReleased(i - 1, BUTTON_A);
#endif
}

//=============================================================================
// �|�[�Y�{�^�������̎擾
//=============================================================================
bool GetPauseButtonTrigger(int i)
{
#ifndef _DEBUG
	if (i == 0)
		return GetKeyboardTrigger(DIK_Q) || IsButtonTriggered(i, BUTTON_M) || IsButtonTriggered(i, BUTTON_R);
	else
		return IsButtonTriggered(i, BUTTON_M) || IsButtonTriggered(i, BUTTON_R);
#else
	if (i == 0)
		return GetKeyboardTrigger(DIK_Q);
	else
		return IsButtonTriggered(i - 1, BUTTON_M);
#endif
}

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	//�}�E�X������
	InitializeMouse(hInst, hWnd);

	//�p�b�h����������
	InitializePad();

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	//�}�E�X�I������
	UninitMouse();

	//�p�b�h�I������
	UninitPad();

	if(g_pDInput)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	//�}�E�X�X�V����
	UpdateMouse();

	//�Q�[���p�b�h�X�V����
	UpdatePad();
}

//=============================================================================
// �L�[�{�[�h�̏���������
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{// �f�o�C�X�I�u�W�F�N�g�̊J��
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState);
	if(SUCCEEDED(hr))
	{
		for(int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			g_aKeyStateRepeat[nCntKey] = g_aKeyStateTrigger[nCntKey];

			if(aKeyState[nCntKey])
			{
				g_aKeyStateRepeatCnt[nCntKey]++;
				if(g_aKeyStateRepeatCnt[nCntKey] >= 20 && g_aKeyStateRepeatCnt[nCntKey] % 5 == 0)
				{
					g_aKeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
				//else
				//{
				//	g_aKeyStateRepeat[nCntKey] = 0;
				//}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCntKey] = 0;
				g_aKeyStateRepeat[nCntKey] = 0;
			}

			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �}�E�X�֌W�̏���
//=============================================================================
// �}�E�X�̏�����
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	// �f�o�C�X�쐬
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// �f�[�^�t�H�[�}�b�g�ݒ�
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// ���̃A�v���Ƌ������[�h�ɐݒ�
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// �}�E�X�̈ړ��l�@���Βl

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// �A�N�Z�X���𓾂�
	pMouse->Acquire();
	return result;
}
//---------------------------------------------------------
void UninitMouse()
{
	if (pMouse)
	{
		pMouse->Unacquire();
		pMouse->Release();
		pMouse = NULL;
	}

}
//-----------------------------------------------------------
HRESULT UpdateMouse()
{
	HRESULT result;
	// �O��̒l�ۑ�
	DIMOUSESTATE2 lastMouseState = mouseState;
	// �f�[�^�擾
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// �}�E�X�̃{�^�����
		for (int i = 0; i<8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// �擾���s
	{
		// �A�N�Z�X���𓾂Ă݂�
		result = pMouse->Acquire();
	}

	return result;

}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// �����ꂽ�Ƃ��ɗ��r�b�g������
}
BOOL IsMouseLeftTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[0] & 0x80);
}
BOOL IsMouseRightPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[1] & 0x80);
}
BOOL IsMouseRightTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[1] & 0x80);
}
BOOL IsMouseCenterPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[2] & 0x80);
}
BOOL IsMouseCenterTriggered(void)
{
	return (BOOL)(mouseTrigger.rgbButtons[2] & 0x80);
}
//------------------
long GetMouseX(void)
{
	return mouseState.lX;
}
long GetMouseY(void)
{
	return mouseState.lY;
}
long GetMouseZ(void)
{
	return mouseState.lZ;
}

//================================================= game pad
//---------------------------------------- �R�[���o�b�N�֐�
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// ���̃f�o�C�X���

}
//---------------------------------------- ������
HRESULT InitializePad(void)			// �p�b�h������
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// �W���C�p�b�h��T��
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// �Z�b�g�����R�[���o�b�N�֐����A�p�b�h�𔭌������������Ă΂��B

	for (i = 0; i < padCount; i++) {
		// �W���C�X�e�B�b�N�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // �f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s

						  // ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // ���[�h�̐ݒ�Ɏ��s

						  // ���̒l�͈̔͂�ݒ�
						  // X���AY���̂��ꂼ��ɂ��āA�I�u�W�F�N�g���񍐉\�Ȓl�͈̔͂��Z�b�g����B
						  // (max-min)�́A�ő�10,000(?)�B(max-min)/2�������l�ɂȂ�B
						  // ����傫������΁A�A�i���O�l�ׂ̍��ȓ�����߂炦����B(�p�b�h�̐��\�ɂ��)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y���͈̔͂�ݒ�
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �e�����ƂɁA�����̃]�[���l��ݒ肷��B
		// �����]�[���Ƃ́A��������̔����ȃW���C�X�e�B�b�N�̓����𖳎�����͈͂̂��ƁB
		// �w�肷��l�́A10000�ɑ΂��鑊�Βl(2000�Ȃ�20�p�[�Z���g)�B
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y���̖����]�[����ݒ�
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//�W���C�X�e�B�b�N���͐���J�n
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- �I������
void UninitPad(void)
{
	for (int i = 0; i < GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ �X�V
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// ������

		result = pGamePad[i]->Poll();	// �W���C�X�e�B�b�N�Ƀ|�[����������
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// �f�o�C�X��Ԃ�ǂݎ��
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		if (dijs.lZ != 0 && dijs.lRx != 0)
		{
			padAxislRx[i] = (float)(dijs.lRx);
			padAxislRy[i] = (float)(dijs.lRy);

		}
		else
		{
			padAxislRx[i] = (float)(dijs.lZ + dijs.lRx);
			padAxislRy[i] = (float)(dijs.lRz + dijs.lRy);

		}

		// �R�Q�̊e�r�b�g�ɈӖ����������A�{�^�������ɉ����ăr�b�g���I���ɂ���
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		//* �`�{�^��
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* �a�{�^��
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* �b�{�^��
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* �w�{�^��
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* �x�{�^��
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* �y�{�^��
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* �k�{�^��
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* �q�{�^��
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* �r�s�`�q�s�{�^��
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* �l�{�^��
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger�ݒ�
		padTrigger[i] = ((lastPadState ^ padState[i])	// �O��ƈ���Ă���
			& padState[i]);					// ��������ON�̂��

		// Release�ݒ�
		padRelease[i] = ((lastPadState ^ padState[i]))	//�O��ƈ���Ă���
			& (~padState[i]);				//��OFF�̂��

											//�X�e�B�b�N��Repeat���菈��
		padAxisYRepeat[i] = GetPadAxisYTriggered(i);

		if (BUTTON_UP & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = 1;
			}
		}
		else if (BUTTON_DOWN & padState[i])
		{
			axisYRepeatCnt[i]++;
			if (axisYRepeatCnt[i] >= 20 && axisYRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisYRepeat[i] = -1;
			}
		}
		else
		{
			axisYRepeatCnt[i] = 0;
			padAxisYRepeat[i] = 0;
		}

		padAxisXRepeat[i] = GetPadAxisXTriggered(i);

		if (BUTTON_RIGHT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = 1;
			}
		}
		else if (BUTTON_LEFT & padState[i])
		{
			axisXRepeatCnt[i]++;
			if (axisXRepeatCnt[i] >= 20 && axisXRepeatCnt[i] % INPUT_SHORTWAIT == 0)
			{
				padAxisXRepeat[i] = -1;
			}
		}
		else
		{
			axisXRepeatCnt[i] = 0;
			padAxisXRepeat[i] = 0;
		}

	}

}

//----------------------------------------------- ����
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}

BOOL IsButtonReleased(int padNo, DWORD button)
{
	return (button & padRelease[padNo]);
}

void CheckAxisXRepeat(int padNo)
{

}

//�X�e�B�b�N�̓��͌��o
int GetPadAxisXTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_RIGHT))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_LEFT))
		return -1;

	else
		return 0;
}

int GetPadAxisYTriggered(int padNo)
{
	if (IsButtonTriggered(padNo, BUTTON_UP))
		return 1;

	else if (IsButtonTriggered(padNo, BUTTON_DOWN))
		return -1;

	else
		return 0;
}

//�p�b�h�̐����o�֐�
int GetPadCount(void)
{
	return padCount;
}

// �E�X�e�B�b�N��x���̒l���l��
float GetStickAxisX(int padNo)
{
	if (padNo >= padCount)
	{
		return 0.0f;
	}

	return (padAxislRx[padNo] / 65535.0f) - 0.5f;
}


// �E�X�e�B�b�N��y���̒l���l��
float GetStickAxisY(int padNo)
{
	if (padNo >= padCount)
	{
		return 0.0f;
	}


	return (padAxislRy[padNo] / 65535.0f) - 0.5f;
}
