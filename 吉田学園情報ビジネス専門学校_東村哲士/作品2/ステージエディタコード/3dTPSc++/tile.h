//====================================================================
//
// タイル処理 (tile.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _TILE_H_
#define _TILE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TILE_SIZE 200.0f

//================================================
// クラス宣言
//================================================

// 3Dポリゴンクラス
class CTile
{
public:
	CTile();
	~CTile();

	// 列挙型宣言 タイルの種類
	typedef enum
	{
		TILE_BLACK = 0,
		TILE_WHITE,
		TILE_RED,
		TILE_GREENERY,
		TILE_SIDEWALK,
		TILE_CHUOLINE,
		TILE_MAX
	}TYPE;

	HRESULT Init(TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }		// 座標設定用
	void SetCol(const D3DXCOLOR col) { m_col = col; };
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetScale(const D3DXVECTOR3 scale) { m_scale = scale; }

	void SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY);
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	void GetTexUV(float* leftX, float* rightX, float* topY, float* bottomY);

	static HRESULT Load(void);
	static void Unload(void);
private:
	static LPDIRECT3DTEXTURE9		m_apTexture[TILE_MAX];
	D3DXMATRIX						m_mtxWorld;		// ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;
	D3DXVECTOR3						m_pos;			// 座標
	D3DXVECTOR3						m_rot;			// 回転
	D3DXVECTOR3						m_scale;
	D3DXCOLOR						m_col;

	float							m_TexLeftX;
	float							m_TexRightX;
	float							m_TexTopY;
	float							m_TexBottomY;

	TYPE							m_type;
};

#endif