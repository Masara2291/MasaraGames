#include "bg.h"
#include "enemy.h"

LPDIRECT3DTEXTURE9 CBg::m_pTexture[BG_NUM] = {};

CBg::CBg(int nPriority) : CObject(nPriority) {

}

CBg::~CBg() {

}

HRESULT CBg::Init(void) {
	for (int nCount = 0; nCount < BG_NUM; nCount++) {
		m_apScene2d[nCount] = CScene2d::Create(CManager::PRIORITY_BG);
		m_apScene2d[nCount]->SetTexture(m_pTexture[nCount]);
		m_apScene2d[nCount]->SetPOS(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y, 0));
		m_apScene2d[nCount]->SetSIZ(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
		m_MoveVector[nCount] = (nCount * 10) + 10;
	}

	return S_OK;
}

void CBg::Uninit(void) {
	for (int nCount = 0; nCount < BG_NUM; nCount++) {
		m_apScene2d[nCount]->Uninit();
	}
}

void CBg::Update(void) {
	CManager::MODE mode = CManager::GetMode();

	if (mode != CManager::MODE_CREATE) {
		for (int nCount = 0; nCount < BG_NUM; nCount++) {
			m_nCounterAnimBg += 5;

			if (m_nCounterAnimBg >= m_MoveVector[nCount]) {
				m_nCounterAnimBg = 0;

				m_nPatternAnimBg++;
				if (m_nPatternAnimBg >= 100) {
					m_nPatternAnimBg = 0;
				}
			}

			m_apScene2d[nCount]->SetTex(FALSE_MOVEMENT, 0.99f, m_nPatternAnimBg);
			m_apScene2d[nCount]->Update();
		}
	}
}

void CBg::Draw(void) {
	for (int nCount = 0; nCount < BG_NUM; nCount++) {
		m_apScene2d[nCount]->Draw();
	}
}

CBg* CBg::Create() {
	CBg* pBg;
	pBg = new CBg(CManager::PRIORITY_BG);

	if (pBg != NULL) {
		pBg->Init();
	}

	return pBg;
}

HRESULT CBg::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_pTexture[0] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bg000.png", &m_pTexture[0]);
	}
	if (m_pTexture[1] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bg001.png", &m_pTexture[1]);
	}
	if (m_pTexture[2] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bg002.png", &m_pTexture[2]);
	}

	return S_OK;
}

void CBg::UnLoad(void) {
	for (int nCount = 0; nCount < BG_NUM; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}