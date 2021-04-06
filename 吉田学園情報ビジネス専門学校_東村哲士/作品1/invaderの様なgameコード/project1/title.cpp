#include "title.h"
#include "CreateStage.h"
#include "keybord.h"
#include "message.h"
#include "player.h"

LPDIRECT3DTEXTURE9 CTitle::m_pTexture[TITLE_NUM] = {};

CTitle::CTitle() {}

CTitle::~CTitle() {}

HRESULT CTitle::Init(void) {
  for (int nCount = 0; nCount < TITLE_NUM; nCount++) {
    m_apScene2d[nCount] = CScene2d::Create();
    m_apScene2d[nCount]->SetTexture(m_pTexture[nCount]);
    m_apScene2d[nCount]->SetPOS(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y, 0));
    m_apScene2d[nCount]->SetSIZ(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
    m_apScene2d[nCount]->SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
  }

  return S_OK;
}

//=============================================================================
//更新処理
//=============================================================================
void CTitle::Update(void) {
  m_pInput = CManager::GetKeybord();

  if (((CKeybord *)m_pInput)
          ->GetkeyboardTrigger(DIK_RETURN)) { // Enterキーを押した時
    CManager::SetMode(CManager::MODE_GAME);   //ゲーム画面に移動
  }
}

//=============================================================================
//終了処理
//=============================================================================
void CTitle::Uninit(void) {
  for (int nCount = 0; nCount < TITLE_NUM; nCount++) {
    m_apScene2d[nCount]->Uninit();
  }
}

//=============================================================================
//描画処理
//=============================================================================
void CTitle::Draw(void) {
  for (int nCount = 0; nCount < TITLE_NUM; nCount++) {
    m_apScene2d[nCount]->Draw();
  }
}

CTitle *CTitle::Create(void) {
  CTitle *pTitle;
  pTitle = new CTitle;

  if (pTitle != NULL) {
    pTitle->Init();
  }

  return pTitle;
}

HRESULT CTitle::Load(void) {
  CRenderere *pRenderere = CManager::GetRenderere();
  if (m_pTexture[0] == NULL) {
    //テクスチャ読み込み
    D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/title000.png",
                              &m_pTexture[0]);
  }

  return S_OK;
}

void CTitle::UnLoad(void) {
  for (int nCount = 0; nCount < TITLE_NUM; nCount++) {
    if (m_pTexture[nCount] != NULL) {
      m_pTexture[nCount]->Release();
      m_pTexture[nCount] = NULL;
    }
  }
}
