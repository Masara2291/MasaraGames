//===============================================
//
// ビルボード処理 (billboard.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================
bool CBillboard::m_bAdditiveSynthesis = false;
bool CBillboard::m_bLay = false;
D3DXVECTOR3	CBillboard::m_pos = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboard::CBillboard(CScene::OBJTYPE objtype) :CScene(objtype)
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_fTexX1 = 0.0f;
    m_fTexX2 = 1.0f;
}

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBillboard::CBillboard()
{
    m_pTexture = NULL;
    m_pVtxBuff = NULL;
    D3DXMatrixIdentity(&m_mtxWorld);
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = DEFAULT_VECTOR;
    m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_fTexX1 = 0.0f;
    m_fTexX2 = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_pos = pos;
    m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    m_size = size;
    m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    VERTEX_3D *pVtx;

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2, -m_size.y / 2, 0.0f);

    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, 1.0f);

    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    m_pVtxBuff->Unlock();

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboard::Uninit(void)
{
    if (m_pVtxBuff != NULL)
    {
        m_pVtxBuff->Release();
        m_pVtxBuff = NULL;
    }

    Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboard::Update(void)
{
    VERTEX_3D *pVtx;

    m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    pVtx[0].pos = D3DXVECTOR3(-m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(+m_size.x / 2, +m_size.y / 2, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(-m_size.x / 2, -m_size.y / 2, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(+m_size.x / 2, -m_size.y / 2, 0.0f);

    pVtx[0].tex = D3DXVECTOR2(m_fTexX1, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(m_fTexX2, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(m_fTexX1, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(m_fTexX2, 1.0f);

    for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
    {
        pVtx[nCount].col = m_col;
        pVtx[nCount].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
    }

    m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;

    // 加算合成
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成にする)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    }

    pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 25);

    //アルファテストの比較方法の設定
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    D3DXMatrixIdentity(&m_mtxWorld);

    if (m_bLay == false)
    {
        D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);
        pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
        D3DXMatrixInverse(&mtxRot, NULL, &mtxRot);

        mtxRot._41 = 0.0f;
        mtxRot._42 = 0.0f;
        mtxRot._43 = 0.0f;

        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
    }
    else
    {
        // 横にする回転
        float fRot = D3DXToRadian(90.0f);

        // 向きを反映
        D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, fRot, m_rot.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

        // 位置を反映
        D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
        D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

        m_bLay = false;
    }

    pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

    pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

    pDevice->SetFVF(FVF_VERTEX_3D);

    pDevice->SetTexture(0, m_pTexture);

    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

    pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    pDevice->SetTexture(0, NULL);

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    // 加算合成を戻す
    if (m_bAdditiveSynthesis == true)
    {
        // レンダーステート(加算合成を戻す)
        pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        // 加算合成のフラグをfalseに戻す
        m_bAdditiveSynthesis = false;
    }
}
