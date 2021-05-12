//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 後藤慎之助
//
//=============================================================================

#define _CRT_SECURE_NO_WARNINGS

#include "game.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "block.h"
#include "input.h"
#include "scene.h"
#include "library.h"
#include "effect3d.h"
#include "field.h"
#include "createstage.h"
#include "ui.h"

//========================================
// マクロ定義
//========================================

// ステージのブロック数
#define MAX_BLOCK 256

// スタートでゲーム状態の管理
#define START_ATTACK 210    // 先攻の攻撃開始

// アニメーションでゲーム状態の管理
#define ANIM_SET_CAMERA 2   // カメラセット
#define ANIM_START 60       // アニメーションスタート
#define ANIM_DAMAGE 150     // ダメージ計算、演出
#define ANIM_END 240        // アニメーション終了

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCharacter *CGame::m_pCharacter = {};

bool CGame::m_bFinishBattle = false;

CGame::STATE CGame::m_state = STATE_CREATESTAGE;

CGame::STAGEDATE m_aStageDate[MAX_BLOCK] = {};

//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{
    m_nCntGameTime = 0;
    m_nCntFinishGame = 0;

    m_pCharacter = NULL;

    // 静的メンバ変数を初期化
    m_bFinishBattle = false;
    m_state = STATE_CREATESTAGE;
}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
    // ブロック
	CCreateStage::LoadStage();
	// 敵
	CCreateStage::LoadEnemy();

    // フィールドの生成
    CField::Create();

    // カメラのロックオン場所
    CCamera::SetCameraLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	CUi::LoadTexture();
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void) {

}