#include "effect3d.h"
#include "manager.h"
#include "renderer.h"

LPDIRECT3DTEXTURE9 CEffect3D::m_apTexture[TYPE_MAX] = {};

CEffect3D::CEffect3D() :CBillboard(CScene::OBJTYPE_EFFECT3D)
{
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_size = DEFAULT_VECTOR;
    m_move = DEFAULT_VECTOR;
    m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    m_type = TYPE_CURSOR;

	m_nLife = 2;
}

CEffect3D::~CEffect3D()
{
}

HRESULT CEffect3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
    m_pos = pos;

    CBillboard::Init(pos, size);
    SetCol(m_col);
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

void CEffect3D::Update(void)
{
    switch (m_type)
    {
    case TYPE_CURSOR:

		m_nLife--;

        // 大きさを反映
        SetSize(m_size);

        break;
    }

    // 移動
    m_pos += m_move;

    // 位置、色を反映
    SetPos(m_pos);
    SetCol(m_col);
    CBillboard::Update();

    // 透明なら消す
    if (m_col.a <= 0.0f || m_nLife <= 0)
    {
        Uninit();
    }
}

void CEffect3D::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    if (m_type == TYPE_CURSOR)
    {
        //CBillboard::SetAdditiveSynthesis();
        CBillboard::SetLay();
    }

    CBillboard::Draw();
}

void CEffect3D::Uninit(void)
{
    CBillboard::Uninit();
}

HRESULT CEffect3D::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor001.png", &m_apTexture[TYPE_CURSOR]);

    return S_OK;
}

void CEffect3D::Unload(void)
{
    for (int nCount = 0; nCount < TYPE_MAX; nCount++)
    {
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, D3DXCOLOR col, TYPE type)
{
    CEffect3D *pEffect3D;
    pEffect3D = new CEffect3D;
    pEffect3D->m_move = move;
    pEffect3D->m_col = col;
    pEffect3D->m_type = type;
    pEffect3D->Init(pos, size, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    return pEffect3D;
}