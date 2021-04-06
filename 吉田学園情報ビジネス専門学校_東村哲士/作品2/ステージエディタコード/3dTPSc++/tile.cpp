//===============================================
//
// 3Dポリゴン処理 (scene3d.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "tile.h"
#include "manager.h"
#include "renderer.h"

//========================
// 静的メンバ変数宣言
//========================
LPDIRECT3DTEXTURE9 CTile::m_apTexture[TILE_MAX] = {};


//=============================================================================
// コンストラクタ
//=============================================================================
CTile::CTile()
{
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_type = TILE_BLACK;

	m_TexLeftX = 0.0f;
	m_TexRightX = 1.0f;
	m_TexTopY = 0.0f;
	m_TexBottomY = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTile::~CTile()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTile::Init(TYPE type)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = type;

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTile::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CTile::Update(void)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - TILE_SIZE / 2, m_pos.y, m_pos.z + TILE_SIZE / 2);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + TILE_SIZE / 2, m_pos.y, m_pos.z + TILE_SIZE / 2);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - TILE_SIZE / 2, m_pos.y, m_pos.z - TILE_SIZE / 2);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + TILE_SIZE / 2, m_pos.y, m_pos.z - TILE_SIZE / 2);

	pVtx[0].tex = D3DXVECTOR2(m_TexLeftX, m_TexTopY);
	pVtx[1].tex = D3DXVECTOR2(m_TexRightX, m_TexTopY);
	pVtx[2].tex = D3DXVECTOR2(m_TexLeftX, m_TexBottomY);
	pVtx[3].tex = D3DXVECTOR2(m_TexRightX, m_TexBottomY);


	for (int nCount = 0; nCount < NUM_VERTEX; nCount++)
	{
		pVtx[nCount].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCount].col = m_col;
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTile::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					// ライトをオフにする

	D3DXMatrixIdentity(&m_mtxWorld);

	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, m_apTexture[m_type]);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetTexture(0, NULL);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					// ライトをオンにする
}

//=============================================================================
// UV座標設定処理
//=============================================================================
void CTile::SetTexUV(const float leftX, const float rightX, const float topY, const float bottomY)
{
	m_TexLeftX = leftX;
	m_TexRightX = rightX;
	m_TexTopY = topY;
	m_TexBottomY = bottomY;
}

//=============================================================================
// UV座標取得用
//=============================================================================
void CTile::GetTexUV(float * leftX, float * rightX, float * topY, float * bottomY)
{
	if (leftX != NULL)
	{
		*leftX = m_TexLeftX;
	}
	if (rightX != NULL)
	{
		*rightX = m_TexRightX;
	}
	if (topY != NULL)
	{
		*topY = m_TexTopY;
	}
	if (bottomY != NULL)
	{
		*bottomY = m_TexBottomY;
	}
}

//=============================================================================
// テクスチャ読み込み処理
//=============================================================================
HRESULT CTile::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Road003.png", &m_apTexture[TILE_BLACK]);
	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field000.png", &m_apTexture[TILE_WHITE]);
	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Road000.png", &m_apTexture[TILE_RED]);
	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/grass000.png", &m_apTexture[TILE_GREENERY]);
	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Road001.png", &m_apTexture[TILE_SIDEWALK]);
	// テクスチャ
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Road002.png", &m_apTexture[TILE_CHUOLINE]);

	return S_OK;
}

//=============================================================================
// テクスチャ破棄処理
//=============================================================================
void CTile::Unload(void)
{
	for (int nCount = 0; nCount < TILE_MAX; nCount++)
	{
		if (m_apTexture[nCount] != NULL)
		{
			m_apTexture[nCount]->Release();
			m_apTexture[nCount] = NULL;
		}
	}
}
