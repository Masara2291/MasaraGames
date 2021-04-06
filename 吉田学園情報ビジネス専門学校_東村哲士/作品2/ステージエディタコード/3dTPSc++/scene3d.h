//====================================================================
//
// シーン上の3Dポリゴン処理 (scene3d.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_3D_SIZE 1000

//================================================
// クラス宣言
//================================================

// 3Dポリゴンクラス
class CScene3D : public CScene
{
public:
    CScene3D();
    CScene3D(OBJTYPE objType);
    virtual ~CScene3D();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindMesh(const LPD3DXMESH pMesh, const  LPD3DXBUFFER pBuffMat, const DWORD nNumMat);
    void BindTexture(const LPDIRECT3DTEXTURE9 pTex);
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

    D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

    static void SetAdditiveSynthesis(void) { m_bAdditiveSynthesis = true; }                              // 加算合成にする

private:
    static bool m_bAdditiveSynthesis;       // 加算合成するかどうか
    LPDIRECT3DTEXTURE9		m_pTexture;
    LPD3DXMESH				m_pMesh;
    LPD3DXBUFFER			m_pBuffMat;
    DWORD					m_nNumMat;
    D3DXMATRIX			    m_mtxWorld;		// ワールドマトリックス
    D3DXVECTOR3				m_pos;			// 座標
    D3DXVECTOR3				m_rot;			// 回転
    D3DXVECTOR3				m_scale;
};

#endif