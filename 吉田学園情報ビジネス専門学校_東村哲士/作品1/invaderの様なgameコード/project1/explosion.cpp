#include "explosion.h"
#include "sound.h"

LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

CExplosion::CExplosion(int nPriority) : CScene2d(nPriority) {

}

CExplosion::~CExplosion() {

}

HRESULT CExplosion::Init(void) {
	CScene2d::SetTex(0.125f, FALSE_MOVEMENT, m_nPatternAnim);
	CScene2d::SetTexture(m_pTexture);
	CScene2d::Init();

	return S_OK;
}

void CExplosion::Uninit(void) {
	CScene2d::Uninit();
}

void CExplosion::Update(void) {
	m_pSound = CManager::GetSound();

	m_nCounterAnim++;
	if (m_nCounterAnim >= 3) {
		m_nCounterAnim = 0;
		m_nPatternAnim++;
	}
	CScene2d::SetTex(0.125f, FALSE_MOVEMENT, m_nPatternAnim);
	CScene2d::SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
	CScene2d::Update();

	if (m_nPatternAnim >= 8) {
		//m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_EXPLOSION);
		m_nPatternAnim = 0;
		Uninit();
	}
}

void CExplosion::Draw(void) {
	CScene2d::Draw();
}

CExplosion* CExplosion::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	CExplosion* pExplosion;
	pExplosion = new CExplosion(CManager::PRIORITY_EXPLOSION);

	if (pExplosion != NULL) {
		pExplosion->Init();

		pExplosion->SetSIZ(siz);
		pExplosion->SetPOS(pos);
	}

	return pExplosion;
}

HRESULT CExplosion::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_pTexture == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/explosion000.png", &m_pTexture);
	}

	return S_OK;
}
void CExplosion::UnLoad(void) {
	if (m_pTexture != NULL) {
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}