//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 樋宮匠
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "input.h"

//================================================
// マクロ定義
//================================================
#define CAMERA_OFFSET 200
#define CAMERA_STATE_LENGTH 180

#define CAMERA_MOVE (7)

#define DISTANCE (80)
#define GREASE_NUM (100)

//カメラの位置
#define CAMERA_POS_X ((2000.0f) / 2)
#define CAMERA_POS_Y (800.0f)
#define CAMERA_POS_Z (-50.0f)

//カメラの注視点
#define CAMERA_ROT_X ((2000.0f) / 2)
#define CAMERA_ROT_Y (-1200.0f)
#define CAMERA_ROT_Z (1000.0f)

// 勝利時のカメラの位置の微調整
#define CAMERA_WIN_POS D3DXVECTOR3(0.0f, 150.0f, 0.0f)
#define CAMERA_WIN_OFFSET 200
#define CAMERA_SENSITIVITY (0.03f)

// カーソル周り
#define CURSOR_POS_Y 5.0f
#define CURSOR_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)	               // 置く場所の大きさ
#define CURSOR_COLLISION_SIZE D3DXVECTOR3(10.0f, 10000.0f, 10.0f)  // 置く場所の当たり判定の大きさ

//================================================
// ライトクラス宣言
//================================================
class CCamera
{
public:
    CCamera();
    ~CCamera();

    typedef enum
    {
        CAMERA_NORMAL = 0,
        CAMERA_PLAYER,
    }CAMERA_STATE;

    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    CCamera *Create(void);
	void InitMouse(void);
    static void SetCameraLookAt(D3DXVECTOR3 pos);
    static void SetCameraState(CAMERA_STATE state) { m_state = state; }

	static D3DXVECTOR3 GetCameraposR(void) { return m_posR; }

private:
    static D3DXVECTOR3	m_posRDest;
    static D3DXVECTOR3	m_posVDest;
	D3DXVECTOR3         m_AmountMovement;
	D3DXVECTOR3         m_StartMousePos;
	int                 m_nType;
    static CAMERA_STATE m_state;
    D3DXVECTOR3			m_posV;
    static D3DXVECTOR3  m_posR;
    D3DXVECTOR3			m_vecU;
    D3DXVECTOR3			m_pos;
	MOUSEWHEEL          m_MouseWheel;
    D3DXMATRIX			m_mtxProjection;
    D3DXMATRIX			m_mtxView;
    static float		m_fR;
    static float		m_fΦ;
    static float		m_fθ;
    int					m_nCountState;
};

#endif