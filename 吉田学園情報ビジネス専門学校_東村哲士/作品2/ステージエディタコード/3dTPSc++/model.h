//====================================================================
//
// シーン上の3Dモデル処理 (model.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

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
class CModel
{
public:
    CModel();
    ~CModel();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(CModel *model);
    void Draw(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
    void BindMesh(const LPD3DXMESH pMesh, const  LPD3DXBUFFER pBuffMat, const DWORD nNumMat);
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
    D3DXVECTOR3 GetRot(void) { return m_rot; }
private:
    LPD3DXMESH				m_pMesh;
    LPD3DXBUFFER			m_pBuffMat;
    DWORD					m_nNumMat;
    D3DXMATRIX			    m_mtxWorld;		// ワールドマトリックス
    D3DXVECTOR3				m_pos;			// 座標
    D3DXVECTOR3				m_rot;			// 回転
    D3DXVECTOR3				m_scale;
};

#endif