#ifndef CREATSTAGE_H
#define CREATSTAGE_H

#include "main.h"
#include "manager.h"
#include "mode.h"
#include "input.h"
#include "scene.h"
#include "field.h"

#include <stdio.h>
#include <string.h>

#define BYTE (256)                                      // 文字列の確保メモリ数
#define BLOCK_SAVE_MAX_NUM (256)                        // 変更内容を保存出来る数
#define OBJECT_MIN_NUM (1)                              // オブジェクトの最低数
#define NEXT_DATA (1) // 操作を戻す場合の、参照する配列
#define NEXT_TIME_DATA (2)
#define DEBUG_TXT_NAME "data/TXT/debug.txt"             // 変更内容を出力するテキストファイル

class CCreateStage : public CMode {
public:
	typedef enum {
		DETAILSCHANGE_NULL = 0,         // 変更内容未設定
		DETAILSCHANGE_CHANGE_POS,       // 座標変更
		DETAILSCHANGE_CHANGE_LAST_POS,  // 座標変更(control + Zを判定)
		DETAILSCHANGE_CHANGE_ROT,       // 向き変更
		DETAILSCHANGE_CHANGE_LAST_ROT,  // 向き変更(control + Zを判定)
		DETAILSCHANGE_BREAK,            // オブジェクト消去
		DETAILSCHANGE_CREATE,           // オブジェクト生成
		DETAILSCHANGE_TOTALELIMINATION, // オブジェクト全消去
		DETAILSCHANGE_MAX               // 項目最大数
	} DETAILSCHANGE;

	typedef enum {
		CREATEOBJECT_STAGE, // ステージ設置モード
		CREATEOBJECT_ENEMY, // 敵設置モード
		CREATEOBJECT_MAX    // モード最大数
	} CREATEOBJECT;

	typedef struct {
		int nNumber;                 // 番号
		int nType;                   // 種類
		bool bChangeJudge;           // 変更を適用したか判定
		DETAILSCHANGE DetailsChange; // 変更内容
		D3DXVECTOR3 pos;             // 座標
		D3DXVECTOR3 rot;             // 向き
		CScene::OBJTYPE ObjectType;  // オブジェクトタイプ
	}BLOCKSAVEDATE;

	CCreateStage();
	~CCreateStage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static HRESULT LoadStage(void);
	static HRESULT SaveStage(void);
	static HRESULT LoadEnemy(void);
	static HRESULT SaveEnemy(void);
	void CursorCollision(D3DXVECTOR3 pos, CREATEOBJECT m_CreateType);
	static CREATEOBJECT GetCreateObject(void) { return m_CreateType; }

	static bool GetFixedSshaft(int nshaft) { return m_bFixedSshaft[nshaft]; }
	static bool GetGreaseJudge(void) { return m_bGreaseJudge; }
	static bool GetSaveJudge(void) { return m_bSaveJudge; }
	static void SetSaveJudge(bool bJudge) { m_bSaveJudge = bJudge; }
	static bool GetPushedControlS(void) { return m_bPushedControlS; }
	static void SetPushedControlS(bool bJudge) { m_bPushedControlS = bJudge; }
	static D3DXVECTOR3 GetCopyPosR(void) { return m_CopyRot; }
	static D3DXVECTOR3 GetCopyPos(void) { return m_CopyPos; }
	static int GetCopynType(void) { return m_nCopyType; }
	static bool GetWriteSaveDetaJudge() { return m_WriteSaveDate; }
	static void SetWriteSaveDataJudge(bool bJudge) { m_WriteSaveDate = bJudge; }
	void CursorControl(D3DXVECTOR3 FixedSshaft, D3DXVECTOR3 CursorPos, CREATEOBJECT m_CreateType);
	void CreatBlockControl(D3DXVECTOR3 FixedSshaft, D3DXVECTOR3 CursorPos, D3DXVECTOR3 PosR, int nType, bool Judge, int nCount, CScene::OBJTYPE ObjectType, DETAILSCHANGE DetallsChange);
	void WriteBlockSaveDate(int nNumAll);
	int UndoingOperation(int nNumAll);
	int RedoUndoOperation(int nNumAll);
	int ShiftArray(int nCount);
	void SetCopyDate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
	void InitCopyDate(void);
	void SaveDate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumber, DETAILSCHANGE DetailsChange, bool bChangeJudge, int nType, int nCount, CScene::OBJTYPE ObjectType);
	int BlockAllDelete(int nCount);
	int EnemyAllDelete(int nCount);
	int SetObject(CInputKeyboard* m_pInputKeyboard, D3DXVECTOR3 m_posR, CMouse* m_pMouse, int nCount, BLOCKSAVEDATE m_aBlockSaveDate, CREATEOBJECT m_CreateType);
	int SetEnemy(CInputKeyboard* m_pInputKeyboard, D3DXVECTOR3 m_posR, CMouse* m_pMouse, int nCount, BLOCKSAVEDATE m_aBlockSaveDate, CREATEOBJECT m_CreateType);
	void InitArray(int nCount);
	void CountDetallsChangeLAST(void);
	HRESULT LoadTextName(void);
	static char* GetTxtName(int nNumber) { return m_cTxtName[nNumber]; }
	void SetObjectType(int nType);
	static int GetBlockType(void) { return m_nBlockType; }
	static int GetEnemyType(void) { return m_nEnemyType; }
	static int GetCopyType(void) { return m_nCopyType; }
	static bool GetPauseJudge(void) { return m_bPauseJudge; }

private:
	static D3DXVECTOR3 m_FieldSiz;                             // フィールドの大きさ
	D3DXVECTOR3 m_posR;                                        // カメラの向き
	D3DXVECTOR3 m_CursorPos;                                   // カーソルの座標
	static D3DXVECTOR3 m_CopyRot;                              // コピーしたオブジェクトの向き
	static D3DXVECTOR3 m_CopyPos;                              // コピーしたオブジェクトの座標
	static int m_nBlockType;                                   // ブロックのタイプ格納用
	static int m_nEnemyType;                                   // 敵のタイプ格納用
	static int m_nCopyType;                                    // コピーしたオブジェクトタイプ格納用
	CMouse* m_pMouse;                                          // マウスのポインタ
	CInputKeyboard* m_pInputKeyboard;                          // キーボードのポインタ
	static bool m_bFixedSshaft[2];                             // 0->x軸, 1->z軸
	D3DXVECTOR3 m_FixedSshaft;                                 // 固定軸
	static bool m_bGreaseJudge;                                // グリッドの使用情報
	static bool m_bSaveJudge;                                  // セーブの成功情報
	static bool m_bPushedControlS;                             // controlとSが押された事を取得
	static BLOCKSAVEDATE m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM]; // 変更内容格納用
	static CREATEOBJECT m_CreateType;                          // 生成するオブジェクトタイプ
	static bool m_bUseAllDelete;                               // 全消去を確認
	static bool m_WriteSaveDate;                               // 変更内容を出力出来たかを取得
	int m_nAllDeleteNum;                                       // 全消去を実行した瞬間の、そのオブジェクトの総数
	int m_nLastCountPos;                                       // 座標変更回数をカウント
	int m_nLastCountRot;                                       // 向き変更回数をカウント
	static char m_cTxtName[3][256];                            // 読むこむファイル名格納用
	static bool m_bPauseJudge;                                 // ポーズ使用判定

	int nAddupAngle = 0;                                       // アングル
	int nAddY = 0;                                             // Y軸追加量
};

#endif
