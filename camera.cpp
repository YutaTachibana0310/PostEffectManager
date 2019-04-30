//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "debugWindow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VIEW_ANGLE					(D3DXToRadian(60.0f))	// 視野角
#define	VIEW_ASPECT					((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z					(10.0f)					// ビュー平面のNearZ値
#define	VIEW_FAR_Z					(5000.0f)				// ビュー平面のFarZ値
#define	VALUE_MOVE_CAMERA			(20.0f)					// カメラの移動量
#define	VALUE_ROTATE_CAMERA			(D3DX_PI * 0.01f)		// カメラの回転量

#define CAMERA_POSITION_OFFSET		(D3DXVECTOR3(0.0f, 30.0f, -150.0f))	//視点の位置
#define CAMERA_TARGET_OFFSET		(D3DXVECTOR3(0.0f, 10.0f, 50.0f))	//注視点の位置
#define CAMERA_OFFSET_MAGNI			(0.2f)								//カメラの移動倍率
#define CAMERA_ROTVALUE_Y			(1.0f)
#define CAMERA_DIST_INITVAL			(150.0f)
#define CAMERA_DIST_MOVEVAL			(0.5f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
CAMERA camera;
static D3DXMATRIX saveView[TARGETPLAYER_MAX];

//=============================================================================
// カメラの初期化
//=============================================================================
HRESULT InitCamera(void)
{
	camera.pos = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
	camera.target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	camera.destPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera.destTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	camera.rot = D3DXVECTOR3(0.0f, -1.378f, 0.0f);
	camera.dist = CAMERA_DIST_INITVAL;

	SetCamera();

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}

#include "input.h"
//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	int x = GetHorizontalInputPress(0);
	int y = GetVerticalInputPress(0);

	camera.rot.y += D3DXToRadian(CAMERA_ROTVALUE_Y * x);
	camera.dist -= y * CAMERA_DIST_MOVEVAL;

	camera.pos.x = cosf(camera.rot.y) * camera.dist;
	camera.pos.z = sinf(camera.rot.y) * camera.dist;

	BeginDebugWindow("Camera");

	DebugText("Pos:%f, %f, %f", camera.pos.x, camera.pos.y, camera.pos.z);
	DebugText("Rot:%f, %f, %f", camera.rot.x, camera.rot.y, camera.rot.z);

	EndDebugWindow("Camera");
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camera.view);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camera.view, 
						&camera.pos,		// カメラの視点
						&camera.target,		// カメラの注視点
						&camera.up);		// カメラの上方向

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camera.view);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camera.projection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&camera.projection,
								VIEW_ANGLE,			// 視野角
								VIEW_ASPECT,		// アスペクト比
								VIEW_NEAR_Z,		// ビュー平面のNearZ値
								VIEW_FAR_Z);		// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camera.projection);
}
//=============================================================================
// ビューマトリックスの取得(画面分割に対応)
//=============================================================================
D3DXMATRIX GetPlayerMtxView(int targetPlayerID)
{
	return saveView[targetPlayerID];
}


//=============================================================================
// カメラの向きの取得
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return camera.rot;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	D3DXMATRIX view;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->GetTransform(D3DTS_VIEW, &view);
	return view;
}

//=============================================================================
// ビューマトリックスの取得
//=============================================================================
D3DXMATRIX GetMtxProjection(void)
{
	D3DXMATRIX projection;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &projection);
	return projection;
}

//=============================================================================
// カメラを向く回転行列の取得
//=============================================================================
void GetInvCameraRotMtx(D3DXMATRIX *mtx, const D3DXVECTOR3* objPos)
{
	if (objPos != NULL)
	{
		D3DXMATRIX inv;
		D3DXMatrixIdentity(&inv);
		D3DXMatrixLookAtLH(&inv, &camera.pos, objPos, &camera.up);
		D3DXMatrixInverse(&inv, NULL, &inv);
		inv._41 = 0.0f;
		inv._42 = 0.0f;
		inv._43 = 0.0f;
	}
	else
	{
		mtx->_11 = camera.view._11;
		mtx->_12 = camera.view._21;
		mtx->_13 = camera.view._31;

		mtx->_21 = camera.view._12;
		mtx->_22 = camera.view._22;
		mtx->_23 = camera.view._32;

		mtx->_31 = camera.view._13;
		mtx->_32 = camera.view._23;
		mtx->_33 = camera.view._33;

		mtx->_41 = 0.0f;
		mtx->_42 = 0.0f;
		mtx->_43 = 0.0f;
	}

}

//=============================================================================
// カメラ座標の取得
//=============================================================================
D3DXVECTOR3 GetCameraPos(void)
{
	return camera.pos;
}

//=============================================================================
// カメラアドレス取得処理
//=============================================================================
CAMERA *GetCameraAdr(void)
{
	return &camera;
}