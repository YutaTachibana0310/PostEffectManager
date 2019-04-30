//=============================================================================
//
// 入力処理 [input.cpp]
// Author : 
//
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX		(256)			// キー最大数
// game pad用設定値
#define DEADZONE		5000			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

#define INPUT_SHORTWAIT (6)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow); // マウスの初期化
void UninitMouse();						// マウスの終了処理
HRESULT UpdateMouse();					// マウスの更新処理

HRESULT InitializePad(void);			// パッド初期化
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UpdatePad(void);
void UninitPad(void);

int GetPadAxisXPressed(int padNo);
int GetPadAxisXTriggered(int padNo);
int GetPadAxisYPressed(int padNo);
int GetPadAxisYTriggered(int padNo);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_aKeyState[NUM_KEY_MAX];			// キーボードの押下状態を保持するワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードのトリガー状態を保持するワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードのリリース状態を保持するワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];		// キーボードのリピート状態を保持するワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//--------------------------------- mouse
static LPDIRECTINPUTDEVICE8 pMouse = NULL; // mouse

static DIMOUSESTATE2   mouseState;		// マウスのダイレクトな状態
static DIMOUSESTATE2   mouseTrigger;	// 押された瞬間だけON

//--------------------------------- game pad

static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// パッドデバイス

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static DWORD	padRelease[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数

//スティックのRepeat状態検出用
static int		padAxisXRepeat[GAMEPADMAX];
static int		padAxisYRepeat[GAMEPADMAX];
static int		lastAxisX[GAMEPADMAX];
static int		lastAxisY[GAMEPADMAX];
static int		axisXRepeatCnt[GAMEPADMAX];
static int		axisYRepeatCnt[GAMEPADMAX];

static float	padAxislRx[GAMEPADMAX];
static float	padAxislRy[GAMEPADMAX];

//=============================================================================
// 水平方向の入力の取得
//=============================================================================
int GetHorizontalInputPress(int i)
{
#ifndef _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
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
	//それ以外はゲームパッドでの操作のみ
	else
	{
		if (IsButtonPressed(i, BUTTON_LEFT))
			return -1;
		
		if (IsButtonPressed(i, BUTTON_RIGHT))
			return 1;

		return 0;
	}
#else
	//0番のプレイヤーはキーボードで操作
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_LEFT))
			return -1;

		if (GetKeyboardPress(DIK_RIGHT))
			return 1;

		return 0;
	}
	//1番以降はパッドで操作
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
// 垂直方向の入力の取得
//=============================================================================
int GetVerticalInputPress(int i)
{
#ifndef _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
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
	//それ以外はゲームパッドでのみ操作可能
	else
	{
		if (IsButtonPressed(i, BUTTON_UP))
			return 1;
		
		if (IsButtonPressed(i, BUTTON_DOWN))
			return -1;

		return 0;
	}
#else
	//0番のプレイヤーはキーボードで操作
	if (i == 0)
	{
		if (GetKeyboardPress(DIK_DOWN))
			return -1;

		if (GetKeyboardPress(DIK_UP))
			return 1;

		return 0;
	}
	//1番以降はパッドで操作
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
// 水平方向の入力の取得
//=============================================================================
int GetHorizontalInputRepeat(int i)
{
#ifndef _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
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
	//それ以外はゲームパッドでのみ操作可能
	else
	{
		return padAxisXRepeat[i];
	}
#else
	//0番のプレイヤーはキーボードで操作
	if (i == 0)
	{
		if (GetKeyboardRepeat(DIK_LEFT))
			return -1;

		if (GetKeyboardRepeat(DIK_RIGHT))
			return 1;

		return 0;
	}
	//1番以降はパッドで操作
	else
	{
		return padAxisXRepeat[i - 1];
	}
#endif
}

//=============================================================================
// 垂直方向の入力の取得
//=============================================================================
int GetVerticalInputRepeat(int i)
{
#ifndef _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
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
	//それ以外はゲームパッドでのみ操作可能
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
// 水平方向の入力の取得
//=============================================================================
int GetHorizontalInputTrigger(int i)
{
#ifndef  _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
	if (i == 0)
	{

		if (GetKeyboardTrigger(DIK_LEFT) || IsButtonTriggered(0, BUTTON_LEFT))
			return -1;

		if (GetKeyboardTrigger(DIK_RIGHT) || IsButtonTriggered(0, BUTTON_RIGHT))
			return 1;

		return 0;
	}
	//それ以外はゲームパッドでのみ操作可能
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
// 垂直方向の入力の取得
//=============================================================================
int GetVerticalInputTrigger(int i)
{
#ifndef _DEBUG
	//0番のプレイヤーのみキーボードでも操作可能
	if (i == 0)
	{
		if (GetKeyboardTrigger(DIK_UP) || IsButtonTriggered(0, BUTTON_UP))
			return 1;

		if (GetKeyboardTrigger(DIK_DOWN) || IsButtonTriggered(0, BUTTON_DOWN))
			return -1;

		return 0;
	}
	//それ以外はゲームパッドでのみ操作可能
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
// 攻撃ボタン押下の取得
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
// 攻撃ボタン押しっぱなしの取得
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
// 攻撃ボタン離しの取得
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
// ポーズボタン押下の取得
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
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	//マウス初期化
	InitializeMouse(hInst, hWnd);

	//パッド初期化処理
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	//マウス終了処理
	UninitMouse();

	//パッド終了処理
	UninitPad();

	if(g_pDInput)
	{// DirectInputオブジェクトの開放
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	//マウス更新処理
	UpdateMouse();

	//ゲームパッド更新処理
	UpdatePad();
}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{// デバイスオブジェクトの開放
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
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
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}

//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow)
{
	HRESULT result;
	// デバイス作成
	result = g_pDInput->CreateDevice(GUID_SysMouse, &pMouse, NULL);
	if (FAILED(result) || pMouse == NULL)
	{
		MessageBox(hWindow, "No mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// データフォーマット設定
	result = pMouse->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Can't setup mouse", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}
	// 他のアプリと協調モードに設定
	result = pMouse->SetCooperativeLevel(hWindow, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse mode error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// デバイスの設定
	DIPROPDWORD prop;

	prop.diph.dwSize = sizeof(prop);
	prop.diph.dwHeaderSize = sizeof(prop.diph);
	prop.diph.dwObj = 0;
	prop.diph.dwHow = DIPH_DEVICE;
	prop.dwData = DIPROPAXISMODE_REL;		// マウスの移動値　相対値

	result = pMouse->SetProperty(DIPROP_AXISMODE, &prop.diph);
	if (FAILED(result))
	{
		MessageBox(hWindow, "Mouse property error", "Warning", MB_OK | MB_ICONWARNING);
		return result;
	}

	// アクセス権を得る
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
	// 前回の値保存
	DIMOUSESTATE2 lastMouseState = mouseState;
	// データ取得
	result = pMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	if (SUCCEEDED(result))
	{
		mouseTrigger.lX = mouseState.lX;
		mouseTrigger.lY = mouseState.lY;
		mouseTrigger.lZ = mouseState.lZ;
		// マウスのボタン状態
		for (int i = 0; i<8; i++)
		{
			mouseTrigger.rgbButtons[i] = ((lastMouseState.rgbButtons[i] ^
				mouseState.rgbButtons[i]) & mouseState.rgbButtons[i]);
		}
	}
	else	// 取得失敗
	{
		// アクセス権を得てみる
		result = pMouse->Acquire();
	}

	return result;

}

//----------------------------------------------
BOOL IsMouseLeftPressed(void)
{
	return (BOOL)(mouseState.rgbButtons[0] & 0x80);	// 押されたときに立つビットを検査
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
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i < padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

						  // モードを設定（フォアグラウンド＆非排他モード）
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // モードの設定に失敗

						  // 軸の値の範囲を設定
						  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
						  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
						  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- 終了処理
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

//------------------------------------------ 更新
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i < padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
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

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* Ｍボタン
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& padState[i]);					// しかも今ONのやつ

		// Release設定
		padRelease[i] = ((lastPadState ^ padState[i]))	//前回と違っていて
			& (~padState[i]);				//今OFFのやつ

											//スティックのRepeat判定処理
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

//----------------------------------------------- 検査
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

//スティックの入力検出
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

//パッドの数検出関数
int GetPadCount(void)
{
	return padCount;
}

// 右スティックのx軸の値を獲得
float GetStickAxisX(int padNo)
{
	if (padNo >= padCount)
	{
		return 0.0f;
	}

	return (padAxislRx[padNo] / 65535.0f) - 0.5f;
}


// 右スティックのy軸の値を獲得
float GetStickAxisY(int padNo)
{
	if (padNo >= padCount)
	{
		return 0.0f;
	}


	return (padAxislRy[padNo] / 65535.0f) - 0.5f;
}
