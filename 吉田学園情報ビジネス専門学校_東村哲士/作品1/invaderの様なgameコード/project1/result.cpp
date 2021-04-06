#include "result.h"

LPDIRECT3DTEXTURE9 CResult::m_pTexture[RESULT_NUM] = {};

CResult::CResult() {

}

CResult::~CResult() {

}

HRESULT CResult::Init(int nNumber) {
	m_pScene2d = CScene2d::Create();
	m_pScene2d->SetTexture(m_pTexture[nNumber]);
	m_pScene2d->SetPOS(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y, 0));
	m_pScene2d->SetSIZ(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));

	return S_OK;
}

void CResult::Uninit(void) {
	m_pScene2d->Uninit();
}

void CResult::Update(void) {
	m_pInput = CManager::GetKeybord();

	if (((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_RETURN)) {//Enterキーを押した時
		if (CManager::GetOldMode() == CManager::MODE_GAME) {
			CManager::SetMode(CManager::MODE_TITLE);//ランキング画面に移動
		}
		if (CManager::GetOldMode() == CManager::MODE_TESTGAME) {
			CManager::SetMode(CManager::MODE_CREATE);//ゲーム画面に移動
		}
		return;
	}

	if (CManager::GetOldMode() == CManager::MODE_TESTGAME) {
		if (((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_LSHIFT)) {
			CManager::SetMode(CManager::MODE_TESTGAME);//ゲーム画面に移動
			return;
		}
	}
}

void CResult::Draw(void) {
	m_pScene2d->Draw();
}

CResult* CResult::Create(int nNumber) {
	CResult* pResult;
	pResult = new CResult;

	if (pResult != NULL) {
		pResult->Init(nNumber);
	}

	return pResult;
}

HRESULT CResult::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_pTexture[0] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result000.png", &m_pTexture[0]);
	}
	if (m_pTexture[1] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result001.png", &m_pTexture[1]);
	}

	return S_OK;
}

void CResult::UnLoad(void) {
	for (int nCount = 0; nCount < RESULT_NUM; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}