#include "scene2d.h"
#include "time.h"
#include "manager.h"
#include "player.h"

CScene2d::CScene2d(int nPriority) : CObject(nPriority){
	m_texpos_x = 0;
	m_texpos_y = 0;
	m_texnum = 0;
}

CScene2d::~CScene2d() {

}

HRESULT CScene2d::Init(void) {
	CRenderere* pRenderere = CManager::GetRenderere();

	//頂点バッファの作成
	pRenderere->GetDevice()->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//固定
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//固定
		&m_pVtxBuff,//変数名によって変化
		NULL);
	VERTEX_2D*pVtx;

	//頂点データ範囲をロックし、頂点バッファへポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	if (m_texpos_x > 0.0f && m_texpos_y < 0) {
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), 1.0f);
	}
	if (m_texpos_y > 0.0f && m_texpos_x < 0) {
		pVtx[0].tex = D3DXVECTOR2(0.0f, m_texpos_y              * m_texnum);
		pVtx[1].tex = D3DXVECTOR2(1.0f, m_texpos_y              * m_texnum);
		pVtx[2].tex = D3DXVECTOR2(0.0f, m_texpos_y + m_texpos_y * m_texnum);
		pVtx[3].tex = D3DXVECTOR2(1.0f, m_texpos_y + m_texpos_y * m_texnum);
	}
	if (m_texpos_x > 0.0f && m_texpos_y > 0.0f) {
		pVtx[0].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), m_texpos_y              * m_texnum);
		pVtx[1].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), m_texpos_y              * m_texnum);
		pVtx[2].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), m_texpos_y + m_texpos_y * m_texnum);
		pVtx[3].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), m_texpos_y + m_texpos_y * m_texnum);
	}

	//頂点座標指定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (PLAYER_DRAW_SIZ_X / 2), m_pos.y - (PLAYER_DRAW_SIZ_Y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (PLAYER_DRAW_SIZ_X / 2), m_pos.y - (PLAYER_DRAW_SIZ_Y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (PLAYER_DRAW_SIZ_X / 2), m_pos.y + (PLAYER_DRAW_SIZ_Y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (PLAYER_DRAW_SIZ_X / 2), m_pos.y + (PLAYER_DRAW_SIZ_Y / 2), 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定(0～255)
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);//aは透明度

	//頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

void CScene2d::Uninit(void) {
	if (m_pVtxBuff != NULL) {
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	Release();
}

void CScene2d::SetTexture(LPDIRECT3DTEXTURE9 pTexture) {
	m_pTexture = pTexture;
}

void CScene2d::SetCOL(D3DCOLOR COL) {
	m_col = COL;
}

void CScene2d::SetSIZ(D3DXVECTOR3 SIZ) {
	m_siz = SIZ;
}

void CScene2d::SetTex(float texpos_x, float texpos_y, int texnum) {
	m_texpos_x = texpos_x;
	m_texpos_y = texpos_y;
	m_texnum = texnum;
}

void CScene2d::Update(void) {
	VERTEX_2D*pVtx;

	//頂点データ範囲をロックし、頂点バッファへポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_siz.x / 2), m_pos.y - (m_siz.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_siz.x / 2), m_pos.y - (m_siz.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_siz.x / 2), m_pos.y + (m_siz.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_siz.x / 2), m_pos.y + (m_siz.y / 2), 0.0f);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	if (m_texpos_x > 0.0f && m_texpos_y < 0) {
		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), 1.0f);
	}
	if (m_texpos_y > 0.0f && m_texpos_x < 0) {
		pVtx[0].tex = D3DXVECTOR2(0.0f, m_texpos_y              * m_texnum);
		pVtx[1].tex = D3DXVECTOR2(1.0f, m_texpos_y              * m_texnum);
		pVtx[2].tex = D3DXVECTOR2(0.0f, m_texpos_y + m_texpos_y * m_texnum);
		pVtx[3].tex = D3DXVECTOR2(1.0f, m_texpos_y + m_texpos_y * m_texnum);
	}
	if (m_texpos_x > 0.0f && m_texpos_y > 0.0f) {
		pVtx[0].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), m_texpos_y              * m_texnum);
		pVtx[1].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), m_texpos_y              * m_texnum);
		pVtx[2].tex = D3DXVECTOR2((m_texpos_x              * m_texnum), m_texpos_y + m_texpos_y * m_texnum);
		pVtx[3].tex = D3DXVECTOR2((m_texpos_x + m_texpos_x * m_texnum), m_texpos_y + m_texpos_y * m_texnum);
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

void CScene2d::Draw(void) {
	CRenderere* pRenderere = CManager::GetRenderere();

	LPDIRECT3DDEVICE9 pDevice = pRenderere->GetDevice();

	if (pDevice != NULL) {
		//頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

void CScene2d::SetPOS(D3DXVECTOR3 pos) { 
	m_pos = pos; 
}

D3DXVECTOR3 CScene2d::GetPos(void) {
	return m_pos;
}

CScene2d* CScene2d::Create(int nPriority) {
	CScene2d* pScene2d;
	pScene2d = new CScene2d(nPriority);
	pScene2d->Init();
	return pScene2d;
}

float CScene2d::GetAngle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2) {
	m_fTmp = atan2((float)(pos1.y - pos2.y), (float)(pos1.x - pos2.x));

	return m_fTmp;
}

bool CScene2d::BooleanToggle(bool bBoolVariable) {
	bBoolVariable = !bBoolVariable;

	return bBoolVariable;
}

D3DCOLOR CScene2d::GetCOL(void) {
	return m_col;
}

D3DXVECTOR3 CScene2d::GetSIZ(void) {
	return m_siz;
}

int CScene2d::GetRand(int min_val, int max_val) {
	return (int)((rand() % (max_val + 1 - min_val)) + min_val);
}