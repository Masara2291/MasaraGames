//====================================================================
//
// ブロックの処理 (character.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//========================================
// マクロ定義
//========================================

#define LOAD_ENEMY_TXT "data/TXT/enemymodel.txt"

//================================================
// クラス宣言
//================================================

// ブロッククラス
class CEnemy : public CScene3D
{
public:

	// 敵の種類
	typedef enum
	{
		TYPE_1,
		TYPE_2,
		TYPE_3,
		TYPE_4,
		TYPE_5,
		TYPE_6,
		TYPE_7,
		TYPE_8,
		TYPE_9,
		TYPE_10,
		TYPE_11,
		TYPE_12,
		TYPE_13,
		TYPE_14,
		TYPE_15,
		TYPE_16,
		TYPE_17,
		TYPE_18,
		TYPE_19,
		TYPE_20,
		TYPE_21,
		TYPE_22,
		TYPE_23,
		TYPE_24,
		TYPE_25,
		TYPE_26,
		TYPE_27,
		TYPE_28,
		TYPE_29,
		TYPE_30,
		TYPE_31,
		TYPE_32,
		TYPE_33,
		TYPE_34,
		TYPE_35,
		TYPE_36,
		TYPE_37,
		TYPE_38,
		TYPE_39,
		TYPE_40,
		TYPE_MAX            // 種類の最大数
	}TYPE;

	// オブジェクト情報
	typedef struct
	{
		char ModelPath[256];
		char TexturePath[256];
		char DrawName[256];
		TYPE Type;
		D3DXVECTOR3 Game_Collision_Siz;
		D3DXVECTOR3 Toul_Collision_Siz;
		bool bUseJudge;
	} ENEMYDATA;

	CEnemy();
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nType);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static void LoadTxt(void);
	static ENEMYDATA GetEnemyData(int nNumber) { return m_aEnemyData[nNumber]; }
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

	D3DXVECTOR3 GetCollisionSize(void) { return m_collisionSize; }          // 衝突の大きさを返す
	bool GetWallRun(void) { return m_bWallRun; }                            // 壁走りできるかどうかを返す

	static CEnemy*GetSelectEnemy(void) { return m_pSelectEnemy; }           // 選択しているブロックを取得
	static void SetSelectEnemy(CEnemy* pBlock) { m_pSelectEnemy = pBlock; } // 選択しているブロックを設定

	int GetType(void) { return m_nType; }
	static int GetEnemyNumAll(void) { return m_nNumAll; }
	int GetNumber(void) { return m_nNumber; }

	static HRESULT SetEnemyData(char* ModelPath, char* TexturePath, char* DrawName, TYPE Type, D3DXVECTOR3 Game_Collision_Siz, D3DXVECTOR3 Toul_Collision_Siz, int nNum, bool bUseJudge);

private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TYPE_MAX];  // 読み込むテクスチャ
	static LPD3DXMESH				m_apMesh[TYPE_MAX];     // 読み込むメッシュ
	static LPD3DXBUFFER				m_apBuffMat[TYPE_MAX];  // 読み込むマテリアルのバッファ
	static DWORD					m_aNumMat[TYPE_MAX];    // 読み込むマテリアル数

	static CEnemy *m_pSelectEnemy;                          // 現在選択しているブロックのポインタ

	D3DXVECTOR3 m_collisionSize;                            // 当たり判定の大きさ
	bool m_bWallRun;                                        // 壁走りできるかどうか
	bool m_bBlink;                                          // 瞬くかどうか
	int m_nCntTime;                                         // 時間を数える
	static int m_nNumAll;                                   // ブロックの総数
	int m_nNumber;                                            // ブロックの番号
	static ENEMYDATA m_aEnemyData[TYPE_MAX];
	static char m_cTextureName[TYPE_MAX][256];                // テクスチャパス保管用変数
	static char m_cModelName[TYPE_MAX][256];                  // モデルパス保管用変数

	int m_nType;                                            // 種類
};

#endif
