//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "mode.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CCharacter;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//ゲーム状態の管理フレーム
#define COUNT_WAIT_FINISH 300	    // ゲーム終了時に、タイトル画面へ行くまでの時間

#define PLAYER1 0

#define PLAYER_SPEED 6.0f

#define WAVE_MAX 2

#define VALUE_KNOCKBACK 50.0f

//プレイヤーの初期位置
#define PLAYER1_FIRST_POS_X (100.0f)
#define PLAYER1_FIRST_POS_Z (1100.0f)

#define MAX_CHARACTER 1

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CMode
{
public:

	// ゲームの状態
	typedef enum
	{
		STATE_CREATESTAGE = 0, // ステージ制作モード
		STATE_MAX			   // 状態の最大数
	}STATE;

	typedef struct
	{
		int nType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} STAGEDATE;

	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static bool GetFinishBattle(void) { return m_bFinishBattle; }           // 戦いが終わったかどうかを取得

	static STATE GetState(void) { return m_state; }                         // ゲームの状態を取得
	static void SetState(STATE state) { m_state = state; }                  // ゲームの状態を設定
	static CCharacter* GetPlayer(void) { return m_pCharacter; }  // プレイヤーを取得

private:
	int m_nCntGameTime;                              // ゲーム時間のカウンタ
	int m_nCntFinishGame;                            // ゲームを終わるまでのカウンタ

	static CCharacter *m_pCharacter;                 // キャラクターのポインタ
	static bool m_bFinishBattle;                     // 戦いが終わったかどうか

	static STATE m_state;                            // 状態
};

#endif