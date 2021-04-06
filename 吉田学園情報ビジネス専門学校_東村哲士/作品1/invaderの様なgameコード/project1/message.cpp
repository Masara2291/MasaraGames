#include "message.h"
#include "animal.h"
#include "boss.h"

LPDIRECT3DTEXTURE9 CMessage::m_pTexture[ERROR_NUM_MAX] = {};
bool CMessage::m_bUseJudge = NULL;

CMessage::CMessage(int nPriority) : CScene2d(nPriority) {

}

CMessage::~CMessage() {
	m_bUseJudge = false;
}

HRESULT CMessage::Init(void) {
	CScene2d::Init();

	return S_OK;
}

void CMessage::Uninit(void) {
	CScene2d::Uninit();
}

void CMessage::Update(void) {
	m_pInput = CManager::GetKeybord();
	D3DXCOLOR col = CScene2d::GetCOL();
	D3DXVECTOR3 siz = CScene2d::GetSIZ();

	if (CManager::GetMode() == CManager::MODE_CREATE) {
		if (col.a < m_Argumentcol.a) {
			col.a += COL_ADD_NUM / 255.0f;
		}

		if (siz.x < MESSAGE_SIZ_X) {
			siz.x += SIZ_ADD_NUM;
			if (siz.x > MESSAGE_SIZ_X) {
				siz.x = MESSAGE_SIZ_X;
			}
		}
		if (siz.y < MESSAGE_SIZ_Y) {
			siz.y += SIZ_ADD_NUM;
			if (siz.y > MESSAGE_SIZ_Y) {
				siz.y = MESSAGE_SIZ_Y;
			}
		}

		if (((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_RETURN)) {
			if (m_ArgumentNum != MESSAGE_BLOCK) {
				while (true) {
					if (siz.x != MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM) {
						if (siz.x > MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM) {
							siz.x -= SIZ_ADD_NUM;
							if (siz.x < MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM) {
								siz.x = MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM;
							}
						}
						if (siz.x < MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM) {
							siz.x += SIZ_ADD_NUM;
							if (siz.x > MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM) {
								siz.x = MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM;
							}
						}
					}

					if (siz.y != MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM) {
						if (siz.y > MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM) {
							siz.y -= SIZ_ADD_NUM;
							if (siz.y < MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM) {
								siz.y = MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM;
							}
						}
						if (siz.y < MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM) {
							siz.y += SIZ_ADD_NUM;
							if (siz.y > MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM) {
								siz.y = MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM;
							}
						}
					}

					col.a -= COL_ADD_NUM / 255;
					if (col.a <= 0) {
						col.a = 0;
					}

					if (siz.x == MESSAGE_SIZ_X - SIZ_SUBTRACTION_NUM && siz.y == MESSAGE_SIZ_Y - SIZ_SUBTRACTION_NUM && col.a == 0) {
						break;
					}
				}
			}

			Uninit();
			return;
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME) {
		switch (m_ArgumentNum) {
		case MESSAGE_WARNING:
			if (m_nJudge == 0) {
				col.a += 10 / 255.0f;
				if (col.a >= 1) {
					col.a = 1;
					m_nJudge = 1;
				}
			}
			if (m_nJudge == 1) {
				col.a -= 10 / 255.0f;
				if (col.a <= 0) {
					col.a = 0;
					m_nJudge = 0;
				}
			}
			if (CBoss::GetNumAll() >= 1) {
				Uninit();
				return;
			}

			break;
		}
	}

	CScene2d::SetSIZ(D3DXVECTOR3(siz));
	CScene2d::SetCOL(col);
	CScene2d::Update();
}

void CMessage::Draw(void) {
	CScene2d::Draw();
}

CMessage* CMessage::Create(MESSAGE_TYPE Message, D3DCOLOR col, D3DXVECTOR3 pos, D3DXVECTOR3 siz) {
	CMessage* pMessage;
	pMessage = new CMessage(CManager::PRIORITY_MESSAGE);

	if (pMessage != NULL) {
		pMessage->Init();
		pMessage->SetTexture(m_pTexture[(int)Message]);
		pMessage->m_ArgumentNum = Message;
		pMessage->SetPOS(D3DXVECTOR3(pos));

		if (siz.x != MESSAGE_SIZ_X && siz.y != MESSAGE_SIZ_Y) {
			pMessage->SetSIZ(D3DXVECTOR3(siz));
		}
		else {
			pMessage->SetSIZ(D3DXVECTOR3(siz.x - SIZ_SUBTRACTION_NUM, siz.y - SIZ_SUBTRACTION_NUM, 0));
		}
		pMessage->m_Argumentsiz = siz;

		if (col != D3DCOLOR_RGBA(255, 255, 255, 255)) {
			pMessage->SetCOL(col);
		}
		else {
			pMessage->SetCOL(D3DCOLOR_RGBA(255, 255, 255, 0));
		}

		pMessage->m_Argumentcol = col;
		pMessage->m_bUseJudge = true;
	}

	return pMessage;
}

HRESULT CMessage::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	//テクスチャ読み込み
	if (m_pTexture[(int)MESSAGE_BLOCK] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bg000.png", &m_pTexture[(int)MESSAGE_BLOCK]);
	}
	if (m_pTexture[(int)MESSAGE_SUCCESS_SAVE_STAGE] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/message001.png", &m_pTexture[(int)MESSAGE_SUCCESS_SAVE_STAGE]);
	}
	if (m_pTexture[(int)MESSAGE_MISS_SAVE_STAGE] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/message002.png", &m_pTexture[(int)MESSAGE_MISS_SAVE_STAGE]);
	}
	if (m_pTexture[(int)MESSAGE_NOT_PUT_ENEMY] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/message003.png", &m_pTexture[(int)MESSAGE_NOT_PUT_ENEMY]);
	}
	if (m_pTexture[(int)MESSAGE_MISS_READ_TXT] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/message004.png", &m_pTexture[(int)MESSAGE_MISS_READ_TXT]);
	}
	if (m_pTexture[(int)MESSAGE_WARNING] == NULL) {
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/message005.png", &m_pTexture[(int)MESSAGE_WARNING]);
	}

	return S_OK;
}

void CMessage::UnLoad(void) {
	for (int nCount = 0; nCount < ERROR_NUM_MAX; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

bool CMessage::GetUseJudge(void) {
	return m_bUseJudge;
}