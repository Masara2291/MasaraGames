//====================================================================
//
// ビルボード処理 (billboard.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BILL_SIZE 50

//================================================
// クラス宣言
//================================================

// ビルボードクラス
class CBillboard : public CScene
{
public:
    CBillboard();
    CBillboard(OBJTYPE objtype);
    virtual ~CBillboard();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindTexture(const LPDIRECT3DTEXTURE9 pTex) { m_pTexture = pTex; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetSize(D3DXVECTOR3 size) { m_size = size; }
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetTexUVX(float x1, float x2) { m_fTexX1 = x1; m_fTexX2 = x2; }
    static D3DXVECTOR3 GetPos(void) { return m_pos; }

    static void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }                              // 加算合成にする
    static void SetLay(void) { m_bLay = true; }                              // 横にする

private:
    static bool m_bAdditiveSynthesis;       // 加算合成するかどうか
    static bool m_bLay;                     // 横にする
    LPDIRECT3DTEXTURE9		m_pTexture;
    D3DXMATRIX			    m_mtxWorld;		// ワールドマトリックス
    LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// 頂点バッファー
    static D3DXVECTOR3		m_pos;			// 座標
    D3DXVECTOR3				m_size;			// 大きさ
    D3DXVECTOR3				m_rot;			// 回転
    D3DXVECTOR3				m_scale;		// 拡大率
    D3DXCOLOR				m_col;
    float					m_fTexX1;
    float					m_fTexX2;
};

#endif