#define _CRT_SECURE_NO_WARNINGS

#include "manager.h"
#include "player.h"
#include "explosion.h"
#include "bg.h"
#include "enemy.h"
#include "title.h"
#include "result.h"
#include "InputMouse.h"
#include "CreateStage.h"
#include "sound.h"
#include "score.h"
#include "number.h"
#include "message.h"
#include "boss.h"

CRenderere* CManager::m_pRenderer     = NULL;
CKeybord* CManager::m_pKeybord        = NULL;
CMouse* CManager::m_pMouse            = NULL;
CPlayer* CManager::m_pPlayer          = NULL;
CBullet* CManager::m_pBullet          = NULL;
CEnemy* CManager::m_pEnemy            = NULL;
CManager::MODE CManager::m_oldmode    = {};
CManager::MODE CManager::m_mode       = {};
CResult* CManager::m_pResult          = NULL;
CTitle* CManager::m_pTitle            = NULL;
CCreateStage* CManager::m_pCreatStage = NULL;
CSound* CManager::m_pSound            = NULL;
CScore* CManager::m_pScore            = NULL;
CBg* CManager::m_pBg                  = NULL;

CManager::CManager() {
	m_mode = MODE_CREATE;
}

CManager::~CManager() {

}

HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd) {
	m_pRenderer = new CRenderere;
	m_pKeybord = new CKeybord;
	m_pMouse = new CMouse;
	m_pSound = new CSound;

	// ‰Šú‰»ˆ—
	if (m_pRenderer->Init(hWnd, true) == S_OK) {
		Load();

		SetMode(m_mode);

		m_pKeybord->Init(hInstance, hWnd);
		m_pMouse->Init(hInstance, hWnd);
		m_pSound->Init(hWnd);

		CCreateStage::LoadStage();

		return S_OK;
	}

	return E_FAIL;
}

void CManager::SetMode(MODE mode) {
	int nJudge;
	bool bJudgeCreateEnemy;
	static int nPosY = 0;
	static int nPosX = 0;
	m_oldmode = m_mode;
	m_mode = mode;
	static int nScore = NULL;

	switch (m_mode) {
	case MODE_TITLE:
		if (m_pResult != NULL) {
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}

		m_pTitle = CTitle::Create();
		break;

	case MODE_TESTGAME:
		m_pSound->SoundPlay(CSound::SOUND_LABEL_BGM000);
		m_pScore = CScore::Create();

		if (m_pCreatStage != NULL) {
			m_pCreatStage->Uninit();
			delete m_pCreatStage;
			m_pCreatStage = NULL;
		}

		if (m_oldmode == MODE_TESTRESULT) {
			CBg::Create();
			CCreateStage::SetEnemy();
			m_pPlayer = CPlayer::Create(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y + (_POLYGON_POS_Y / 2), 0), CObject::OBJTYPE_PLAYER_DRAW);
		}
		break;

	case MODE_GAME:
		if (m_pTitle != NULL) {
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}

		m_pPlayer = CPlayer::Create(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y + (_POLYGON_POS_Y / 2), 0), CObject::OBJTYPE_PLAYER_DRAW);

		//ƒXƒRƒA¶¬‚Æ‰Šú‰»
		m_pScore = CScore::Create();
		if (nScore != NULL && m_oldmode != MODE_RESULT || m_oldmode != MODE_TESTRESULT && m_pPlayer->GetDeathStage(0) == 0 && m_pPlayer->GetDeathStage(1) == 0) {
			if (nScore >= 100) {
				m_pScore->SetScore(nScore / 2);
			}
			else {
				m_pScore->SetScore(nScore);
			}
		}

		m_pCreatStage = new CCreateStage;
		m_pCreatStage->Init();

		m_pCreatStage->CreateStage();

		m_pSound->SoundPlay(CSound::SOUND_LABEL_BGM000);

		CBg::Create();

		bJudgeCreateEnemy = true;

		//“G‚Ìî•ñ‘S‚Äæ“¾‚µ‚Ä“G‚ª1‘ÌˆÈã“o˜^‚³‚ê‚Ä‚¢‚éê‡A‚»‚Ì“G‚ğ¶‘¶ó‘Ô‚É•ÏX‚·‚é
		//“G‚ª1‘Ì‚à“o˜^‚³‚ê‚Ä‚¢‚È‚¢ê‡A“G4‘Ì‚ğ“o˜^‚·‚é
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			CObject* pObject = CObject::GetScene(PRIORITY_ENEMY, nCount);
			if (pObject != NULL) {
				CObject::OBJTYPE ObjType = pObject->GetObjType();
				if (ObjType == CObject::OBJTYPE_ENEMY) {
					bJudgeCreateEnemy = false;
				}
			}
		}
		if (bJudgeCreateEnemy == true) {
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0), CEnemy::ENEMY_TYPE(0), CObject::OBJTYPE_ENEMY);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0), CEnemy::ENEMY_TYPE(1), CObject::OBJTYPE_ENEMY);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0), CEnemy::ENEMY_TYPE(2), CObject::OBJTYPE_ENEMY);
			CEnemy::Create(D3DXVECTOR3(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 0), CEnemy::ENEMY_TYPE(3), CObject::OBJTYPE_ENEMY);
		}
		if (bJudgeCreateEnemy == false) {
			CCreateStage::SetEnemy();
		}

		break;

	case MODE_CREATE:
		if (m_pResult != NULL) {
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}

		m_pSound->StopSound();

		for (int nCount = 0; nCount < CAnimal::ENEMYTYPE_MAX; nCount++) {
			m_pEnemy->Create(D3DXVECTOR3(0 + ((ENEMY_SIZ / 2) + (nCount * ENEMY_SIZ)), 0 + (ENEMY_SIZ / 2), 0), (CEnemy::ENEMY_TYPE)nCount, CObject::OBJTYPE_ENEMY_CREAT);
		}

		m_pCreatStage = new CCreateStage;
		m_pCreatStage->Init();

		CBg::Create();

		m_pCreatStage->CreateStage();
		CCreateStage::SetEnemy();

		m_pPlayer = CPlayer::Create(D3DXVECTOR3(_POLYGON_POS_X, _POLYGON_POS_Y + (_POLYGON_POS_Y / 2), 0), CObject::OBJTYPE_PLAYER_DRAW);

		break;

	case MODE_TESTRESULT:
	case MODE_RESULT:
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM000);

		nJudge = 0;
		//“G‚Ìî•ñ‚ğ‘S‚Äæ“¾‚µA¶‘¶ó‘Ô‚Ì“G‚ª‚¢‚é‚©”»’è
		//¶‘¶‚µ‚Ä‚é“G‚ª‚¢‚éê‡A‚»‚Ì“G‚ğ€–Só‘Ô‚É•Ï‚¦‚ÄƒŠƒUƒ‹ƒg‰æ–Ê‚Ì‰æ‘œ‚ğØ‚è‘Ö‚¦‚é
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			CObject* pObject = CObject::GetScene(PRIORITY_ENEMY, nCount);
			if (pObject != NULL) {
				CObject::OBJTYPE ObjType = pObject->GetObjType();
				if (ObjType == CObject::OBJTYPE_ENEMY && ((CEnemy*)pObject)->m_State == CAnimal::STATE_LIFE) {
					nJudge = 1;
					((CEnemy*)pObject)->m_State = CAnimal::STATE_DEATH;
					((CEnemy*)pObject)->AddNumAll(-1);
				}
			}

			pObject = CObject::GetScene(PRIORITY_BOSS, nCount);
			if (pObject != NULL) {
				CObject::OBJTYPE ObjType = pObject->GetObjType();
				if (ObjType == CObject::OBJTYPE_BOSS && ((CEnemy*)pObject)->m_State == CAnimal::STATE_LIFE) {
					nJudge = 1;
					((CEnemy*)pObject)->m_State = CAnimal::STATE_DEATH;
					((CEnemy*)pObject)->AddNumAll(-1);
				}
			}
		}

		//ƒXƒRƒA‚ğ•Û‘¶‚·‚é
		if (CScore::GetScore() > 0 && m_pPlayer->GetDeathStage(0) >= CCreateStage::MAP_1) {
			nScore = CScore::GetScore();
		}

		CObject::UninitSome(CObject::OBJTYPE_ENEMY, true);

		m_pResult = CResult::Create(nJudge);

		break;
	}
}

void CManager::Update(void) {
	switch (m_mode) {
	case MODE_TITLE:
		m_pTitle->Update();
		break;

	case MODE_GAME:
		break;

	case MODE_CREATE:
		m_pCreatStage->Update();
		break;

	case MODE_TESTRESULT:
	case MODE_RESULT:
		m_pResult->Update();
		break;
	}

	m_pRenderer->Update();
	m_pKeybord->Update();
	m_pMouse->Update();
}

void CManager::Draw(void) {
	switch (m_mode) {
	case MODE_TITLE:
		m_pTitle->Draw();
		break;

	case MODE_GAME:
		break;

	case MODE_CREATE:
		m_pCreatStage->Draw();
		break;

	case MODE_TESTRESULT:
	case MODE_RESULT:
		m_pResult->Draw();
		break;
	}

	m_pRenderer->Draw();
}

void CManager::Uninit(void) {
	m_pRenderer->Uninit();
	delete m_pRenderer;

	m_pKeybord->Uninit();
	delete m_pKeybord;

	m_pMouse->Uninit();
	delete m_pMouse;

	m_pSound->Uninit();
	delete m_pSound;

	UnLoad();
	CObject::UninitAll();

	FILE* fp;
	fp = fopen(DEATH_STAGE_TXT, "w");
	if (fp != NULL) {
		fprintf(fp, "%d", 0);

		fclose(fp);
	}
}

void CManager::Load(void) {
	CBullet::Load();
	CPlayer::Load();
	CExplosion::Load();
	CBg::Load();
	CEnemy::Load();
	CTitle::Load();
	CResult::Load();
	CCreateStage::Load();
	CNumber::Load();
	CMessage::Load();
	CBoss::Load();
}

void CManager::UnLoad(void) {
	CBullet::UnLoad();
	CPlayer::UnLoad();
	CExplosion::UnLoad();
	CBg::UnLoad();
	CEnemy::UnLoad();
	CTitle::UnLoad();
	CResult::UnLoad();
	CCreateStage::UnLoad();
	CNumber::UnLoad();
	CMessage::UnLoad();
	CBoss::UnLoad();
}

//=============================================================================
// ƒfƒoƒCƒXî•ñ‚ğ•Ô‚·
//=============================================================================
CManager::MODE CManager::GetMode(void) {
	return m_mode;
}

CManager::MODE CManager::GetOldMode(void) {
	return m_oldmode;
}

CRenderere* CManager::GetRenderere(void) {
	return m_pRenderer;
}

CKeybord* CManager::GetKeybord(void) {
	return m_pKeybord;
}

CMouse* CManager::GetMouse(void) {
	return m_pMouse;
}

CPlayer* CManager::GetPlayer(void) {
	return m_pPlayer;
}

CBullet* CManager::GetBullet(void) {
	return m_pBullet;
}

CEnemy* CManager::GetEnemy(void) {
	return m_pEnemy;
}

CSound* CManager::GetSound(void) {
	return m_pSound;
}

CScore* CManager::GetScore(void) {
	return m_pScore;
}