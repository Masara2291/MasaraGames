//===============================================
//
// 光源処理 (camera.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "camera.h"

#include "CreateStage.h"
#include "block.h"
#include "effect3d.h"
#include "game.h"
#include "input.h"
#include "library.h"
#include "manager.h"
#include "renderer.h"

//========================
// 静的メンバ変数宣言
//========================
D3DXVECTOR3 CCamera::m_posRDest        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CCamera::m_posVDest        = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CCamera::CAMERA_STATE CCamera::m_state = CCamera::CAMERA_NORMAL;

D3DXVECTOR3 CCamera::m_posR = D3DXVECTOR3(0, 0, 0);

//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{
    D3DXMatrixIdentity(&m_mtxView);
    D3DXMatrixIdentity(&m_mtxProjection);
    m_pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_state       = CAMERA_NORMAL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
    InitMouse();

    POINT Point;
    GetCursorPos(&Point);

    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_posV           = D3DXVECTOR3(0.0f, 30.0f, -30.0f);    //位置
    m_posR           = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       //注視点
    m_vecU           = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_StartMousePos  = D3DXVECTOR3(Point.x, Point.y, 0.0f);
    m_AmountMovement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_state          = CAMERA_NORMAL;

    D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
    pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

    D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 2000.0f);
    pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	if (CCreateStage::GetPauseJudge() == false) {
		//マウスの座標取得
		POINT Point;
		GetCursorPos(&Point);
		D3DXVECTOR3 MousePos = D3DXVECTOR3(Point.x, Point.y, 0);
		//キーボードの確保したメモリを取得
		CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
		//  カメラ位置修正処理
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		//マウスホイールの情報を取得
		m_MouseWheel = GetMouseWheel();

		// マウスの移動量を計算
		m_AmountMovement.y += (m_StartMousePos.y - MousePos.y) * CAMERA_SENSITIVITY;
		m_AmountMovement.x += (MousePos.x - m_StartMousePos.x) * CAMERA_SENSITIVITY;

		// 上・下を見すぎないように調整
		if (m_AmountMovement.y >= 89) {
			m_AmountMovement.y = 89;
		}
		if (m_AmountMovement.y <= -89) {
			m_AmountMovement.y = -89;
		}

		// 左コントロールが押されていない場合
		if (!pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
			// グリッジを使用していない場合
			if (CCreateStage::GetGreaseJudge() == false) {
				if (pInputKeyboard->GetKeyboardPress(DIK_W)) {
					m_posV.x += sinf(D3DXToRadian(m_AmountMovement.x)) * 30.0f;
					m_posV.z += cosf(D3DXToRadian(m_AmountMovement.x)) * 30.0f;
				}
				if (pInputKeyboard->GetKeyboardPress(DIK_S)) {
					m_posV.x -= sinf(D3DXToRadian(m_AmountMovement.x)) * 30.0f;
					m_posV.z -= cosf(D3DXToRadian(m_AmountMovement.x)) * 30.0f;
				}
				if (pInputKeyboard->GetKeyboardPress(DIK_A)) {
					m_posV.x -= sinf(D3DXToRadian(m_AmountMovement.x + 90)) * 30.0f;
					m_posV.z -= cosf(D3DXToRadian(m_AmountMovement.x + 90)) * 30.0f;
				}
				if (pInputKeyboard->GetKeyboardPress(DIK_D)) {
					m_posV.x += sinf(D3DXToRadian(m_AmountMovement.x + 90)) * 30.0f;
					m_posV.z += cosf(D3DXToRadian(m_AmountMovement.x + 90)) * 30.0f;
				}
			}
			// グリッジを使用している場合
			if (CCreateStage::GetGreaseJudge() == true) {
				if (pInputKeyboard->GetKeyboardTrigger(DIK_W)) {
					m_posV.x += sinf(D3DXToRadian(m_AmountMovement.x)) * GREASE_NUM;
					m_posV.z += cosf(D3DXToRadian(m_AmountMovement.x)) * GREASE_NUM;
				}
				if (pInputKeyboard->GetKeyboardTrigger(DIK_S)) {
					m_posV.x -= sinf(D3DXToRadian(m_AmountMovement.x)) * GREASE_NUM;
					m_posV.z -= cosf(D3DXToRadian(m_AmountMovement.x)) * GREASE_NUM;
				}
				if (pInputKeyboard->GetKeyboardTrigger(DIK_A)) {
					m_posV.x -= sinf(D3DXToRadian(m_AmountMovement.x + 90)) * GREASE_NUM;
					m_posV.z -= cosf(D3DXToRadian(m_AmountMovement.x + 90)) * GREASE_NUM;
				}
				if (pInputKeyboard->GetKeyboardTrigger(DIK_D)) {
					m_posV.x += sinf(D3DXToRadian(m_AmountMovement.x + 90)) * GREASE_NUM;
					m_posV.z += cosf(D3DXToRadian(m_AmountMovement.x + 90)) * GREASE_NUM;
				}
			}
			// スペース/左シフトでカメラのY軸を上下させる
			if (pInputKeyboard->GetKeyboardPress(DIK_SPACE)) {
				m_posV.y += 35.0f;
			}
			if (pInputKeyboard->GetKeyboardPress(DIK_LSHIFT)) {
				m_posV.y -= 35.0f;
			}
		}

		// エンターキーが押された場合
		if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)) {
			// カメラを初期化
			Init();
		}

		//ホイールを使用する
		if (m_MouseWheel.zDelta >= 120) {
			m_posV.y += 70.0f;
		}
		if (m_MouseWheel.zDelta <= -120) {
			m_posV.y -= 70.0f;
		}

		// 注視点を計算
		m_posR.x = m_posV.x + sinf(D3DXToRadian(m_AmountMovement.x)) * DISTANCE;
		m_posR.y = m_posV.y + tanf(D3DXToRadian(m_AmountMovement.y)) * DISTANCE;
		m_posR.z = m_posV.z + cosf(D3DXToRadian(m_AmountMovement.x)) * DISTANCE;

		D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

		D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(90), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 50000.0f);
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

		InitMouse();
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CCamera *CCamera::Create(void)
{
    CCamera *pCamera = NULL;
    pCamera          = new CCamera;
    pCamera->Init();
    pCamera->m_nType = 0;
    return pCamera;
}

//=============================================================================
// 注視点変更処理
//=============================================================================
void CCamera::SetCameraLookAt(D3DXVECTOR3 pos)
{
    m_posRDest = D3DXVECTOR3(pos.x, pos.y, pos.z);
}

//=============================================================================
// マウスの座標をウィンドウの中心に移動させる
//=============================================================================
void CCamera::InitMouse(void)
{
    HWND hDesktop = GetDesktopWindow();

    WINDOWINFO windowInfo;
    LONG pos_x, pos_y;
    windowInfo.cbSize = sizeof(WINDOWINFO);

    GetWindowInfo(hDesktop, &windowInfo);
    pos_x = windowInfo.rcWindow.left;
    pos_y = windowInfo.rcWindow.top;
    SetCursorPos(SCREEN_WIDTH / 2 + pos_x, SCREEN_HEIGHT / 2 + pos_y);
}
