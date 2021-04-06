//=============================================================================
//
// ダメージ表示処理 [damage.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CEffect3D :public CBillboard
{
public:

    // UIの種類
    typedef enum
    {
        TYPE_CURSOR,            // 置く場所
        TYPE_MAX			    // 種類の最大数
    }TYPE;

    CEffect3D();
    ~CEffect3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CEffect3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type);

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];
    D3DXVECTOR3 m_pos;
    D3DXVECTOR3 m_size;
    D3DXVECTOR3 m_move;
    D3DXCOLOR m_col;
	int m_nLife;
    TYPE m_type;
};
#endif
