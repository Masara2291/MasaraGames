//====================================================================
//
// フィールドの処理 (character.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "tile.h"
#include "CreateStage.h"

//================================================
// マクロ定義
//================================================

#define FIELD_POS (D3DXVECTOR3(2000.0f, 0.0f, 4800.0f))  // フィールドの位置
#define FIELD_SCALE (D3DXVECTOR3(100.0f, 1.0f, 100.0f))  // フィールドのスケール

#define MAP_WIDTH	75	// マップの幅
#define MAP_HEIGHT	75	// マップの奥行

#define NUM_TILE	(MAP_WIDTH * MAP_HEIGHT)

#define CSV_FIELD_NAME (CSV_TXT_NAME)

//================================================
// クラス宣言
//================================================

// フィールドクラス
class CField : public CScene
{
public:
    CField();
    ~CField();

    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
    void Uninit(void);
	void Update(void);
	void Draw(void);

	void LoadMapData(void);
	static void SetMapSiz(D3DXVECTOR3 pos) { m_siz = pos; }

	static CField *Create(void);

	static void SetField(void);
private:
	static D3DXVECTOR3 m_siz;
	static CTile*      m_pTile[MAP_WIDTH][MAP_HEIGHT];
	static int	       m_aGround[MAP_WIDTH][MAP_HEIGHT];
};

#endif