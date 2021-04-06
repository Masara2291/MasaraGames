#define _CRT_SECURE_NO_WARNINGS

#include "CreateStage.h"
#include "InputMouse.h"
#include "keybord.h"
#include "player.h"
#include "bg.h"
#include "message.h"
#include "math.h"

CCreateStage::STAGEDATE m_aStageDate[CCreateStage::MAP_MAX][CCreateStage::STAGE_MAX][MAX_NUM] = {};
bool CCreateStage::m_bEnemyBox = false;

char* m_aFileName[CCreateStage::MAP_MAX][CCreateStage::STAGE_MAX] = {
	{ { "stage/0/0-0.txt"},
	  { "stage/0/0-1.txt"},
	  { "stage/0/0-2.txt"},
	  { "stage/0/0-3.txt"},},

	{ { "stage/1/1-0.txt" },
	  { "stage/1/1-1.txt" },
	  { "stage/1/1-2.txt" },
	  { "stage/1/1-3.txt" }, },

	{ { "stage/2/2-0.txt" },
	  { "stage/2/2-1.txt" },
	  { "stage/2/2-2.txt" },
	  { "stage/2/2-3.txt" }, },

	{ { "stage/3/3-0.txt" },
	  { "stage/3/3-1.txt" },
	  { "stage/3/3-2.txt" },
	  { "stage/3/3-3.txt" }, },
};

LPDIRECT3DTEXTURE9 CCreateStage::m_pTexture[MAX_TEXTURE] = {};

CCreateStage::CCreateStage() {
	if (CManager::GetMode() == CManager::MODE_CREATE) {
		m_EnemyDate.nType = {};
		m_EnemyDate.pos = D3DXVECTOR3(((CEnemy*)CObject::GetScene(CManager::PRIORITY_ENEMY_CREAT, 0))->GetPos().x,
			((CEnemy*)CObject::GetScene(CManager::PRIORITY_ENEMY_CREAT, 0))->GetPos().y,
			0);

	}
}

CCreateStage::~CCreateStage() {

}

HRESULT CCreateStage::Init(void) {
	if (CManager::GetMode() == CManager::MODE_CREATE) {
		m_pInput = CManager::GetMouse();
		CMouse::MSTATE* Mouse = CMouse::GetMouse();

		for (int nCount = 0; nCount < MAX_TEXTURE; nCount++) {
			m_pScene2d01[nCount] = CScene2d::Create(CManager::PRIORITY_CURSOR);
			m_pScene2d01[nCount]->SetTexture(m_pTexture[nCount]);
		}

		m_pScene2d01[0]->SetSIZ(D3DXVECTOR3(CURSOR_SIZ, CURSOR_SIZ, 0));
		m_pScene2d01[1]->SetSIZ(D3DXVECTOR3(CURSOR_SIZ, CURSOR_SIZ / 2, 0));
		m_pScene2d01[0]->SetPOS(D3DXVECTOR3(Mouse->x, Mouse->y, 0));
		m_pScene2d01[1]->SetPOS(D3DXVECTOR3(m_EnemyDate.pos.x, m_EnemyDate.pos.y, 0));
	}

	return S_OK;
}

void CCreateStage::Uninit(void) {
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++) {
		m_pScene2d01[nCount]->Uninit();
	}
}

void CCreateStage::Update(void) {
	D3DXVECTOR3 Judge_pos;
	m_pInput = CManager::GetMouse();
	CMouse::MSTATE* Mouse = CMouse::GetMouse();
	CObject* PlayerObject = NULL;

	if (PlayerObject == NULL) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			PlayerObject = CObject::GetScene(CManager::PRIORITY_PLAYER, nCount);
			if (PlayerObject != NULL && PlayerObject->GetObjType() == CObject::OBJTYPE_PLAYER_DRAW) {
				break;
			}
		}
	}

	m_pos = D3DXVECTOR3(Mouse->x, Mouse->y, 0);

	//マス事に動かす時の位置を探索->画像の位置をマス事の中心に置く
	if (m_bEnemyBox == false) {
		for (int nCountX = (CURSOR_SIZ / 2); nCountX < SCREEN_WIDTH; nCountX += CURSOR_SIZ) {
			for (int nCountY = (CURSOR_SIZ / 2); nCountY < SCREEN_HEIGHT; nCountY += CURSOR_SIZ) {
				if (m_pos.x >= nCountX - (CURSOR_SIZ / 2) &&
					m_pos.x <= nCountX + (CURSOR_SIZ / 2) &&
					m_pos.y >= nCountY - (CURSOR_SIZ / 2) &&
					m_pos.y <= nCountY + (CURSOR_SIZ / 2)) {

					m_pos.x = nCountX;
					m_pos.y = nCountY;
					break;
				}
			}
		}
	}

	//左クリックされた時
	if (((CMouse*)m_pInput)->GetMouseTriggerLeft()) {
		//敵のタブを開いてる場合
		if (m_bEnemyBox == true &&
			CMath::GetEnemyDate(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY_CREAT, D3DXVECTOR2(8, 8), D3DXVECTOR2(1, 1), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2(-1, 0)).nType != FALSE_MOVEMENT &&
			CMath::GetEnemyDate(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY_CREAT, D3DXVECTOR2(8, 8), D3DXVECTOR2(1, 1), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2(-1, 0)).pos != NULL) {
			m_EnemyDate = CMath::GetEnemyDate(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY_CREAT, D3DXVECTOR2(8, 8), D3DXVECTOR2(1, 1), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2(-1, 0));
		}

		//敵のタブを閉じてる場合
		if (m_bEnemyBox == false) {
			//消しゴム以外なら敵を設置
			if (m_EnemyDate.nType != CEnemy::ERASER_0) {
				//画面端と自機近くに置けなくする条件式
				if (m_pos.x > (CURSOR_SIZ / 2) &&
					m_pos.x < SCREEN_WIDTH - (CURSOR_SIZ / 2) &&
					m_pos.y >(CURSOR_SIZ / 2) &&
					m_pos.y < SCREEN_HEIGHT - (CURSOR_SIZ / 2) &&
					m_pos.y < ((CPlayer*)PlayerObject)->GetPos().y - (PLAYER_DRAW_SIZ_Y * 3)) {
					//重なってるかを判定
					bool bOverlap = false;

					bOverlap = CMath::GetOverlapJudge(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY, D3DXVECTOR2(8, 2), D3DXVECTOR2(8, 3), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2 (-1, FALSE_ADD));

					//重ならない場合敵を設置
					if (bOverlap == false) {
						m_pEnemy->Create(D3DXVECTOR3(m_pos.x, m_pos.y - (CURSOR_SIZ / 2), 0), m_EnemyDate.nType, CObject::OBJTYPE_ENEMY);
					}
				}
			}

			//消しゴムならマスの中にいる敵を消す
			if (m_EnemyDate.nType == CEnemy::ERASER_0 &&
				CMath::GetObjectDate(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY, D3DXVECTOR2(8, 2), D3DXVECTOR2(8, 3), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2(-1, FALSE_ADD)) != NULL) {
				CMath::GetObjectDate(D3DXVECTOR3(m_pos), CManager::PRIORITY_ENEMY, D3DXVECTOR2(8, 2), D3DXVECTOR2(8, 3), D3DXVECTOR2(2, 2), D3DXVECTOR2(2, 2), D3DXVECTOR2(FALSE_ADD, FALSE_ADD), D3DXVECTOR2(-1, FALSE_ADD))->Uninit();
			}
		}
	}

	m_pScene2d01[0]->SetPOS(D3DXVECTOR3(m_pos.x, m_pos.y, 0));
	m_pScene2d01[1]->SetPOS(D3DXVECTOR3(m_EnemyDate.pos.x, m_EnemyDate.pos.y, 0));

	if (m_bEnemyBox == true) {
		m_pScene2d01[1]->SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	if (m_bEnemyBox == false) {
		m_pScene2d01[1]->SetCOL(D3DCOLOR_RGBA(0, 0, 0, 0));
	}

	m_pInput = CManager::GetKeybord();
	if (((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_RETURN) && CMessage::GetUseJudge() == false && CEnemy::GetNumAll() > 0) {
		m_bEnemyBox = false;
		CManager::SetMode(CManager::MODE_TESTGAME);
		CEnemy::m_nCreatEnemyNum = 0;
		return;
	}

	if (((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_1)) {
		m_bEnemyBox = m_pScene2d02->BooleanToggle(m_bEnemyBox);
	}

	if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_S) && ((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_LCONTROL) ||
		((CKeybord*)m_pInput)->GetkeyboardPress(DIK_LCONTROL) && ((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_S)) {

		if (CEnemy::GetNumAll() > 0) {
			SaveStage();
		}
		else {
			//メッセージの画像を生成
			CMessage::Create(CMessage::MESSAGE_BLOCK, D3DCOLOR_RGBA(255, 255, 255, 155), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
			CMessage::Create(CMessage::MESSAGE_NOT_PUT_ENEMY);
		}
	}
}

void CCreateStage::Draw(void) {
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++) {
		m_pScene2d01[nCount]->Draw();
	}
}

HRESULT CCreateStage::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_pTexture[0] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/Cursor000.png", &m_pTexture[0]);
	}
	if (m_pTexture[1] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/choice000.png", &m_pTexture[1]);
	}

	return S_OK;
}

void CCreateStage::UnLoad(void) {
	for (int nCount = 0; nCount < MAX_TEXTURE; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

HRESULT CCreateStage::SaveStage(void) {
	FILE* fp;
	fp = fopen(TEXT_NUM, "w");

	if (fp != NULL) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			CObject* pObject = CObject::GetScene(CManager::PRIORITY_ENEMY, nCount);
			if (pObject != NULL) {
				fprintf(fp, "%lf\n%lf\n%lf\n%d\n", ((CEnemy*)pObject)->GetPos().x, ((CEnemy*)pObject)->GetPos().y, ((CEnemy*)pObject)->GetPos().z, (int)(((CEnemy*)pObject)->GetEnemyType()));
			}
		}

		fclose(fp);

		//メッセージの画像を生成
		CMessage::Create(CMessage::MESSAGE_BLOCK, D3DCOLOR_RGBA(255, 255, 255, 155), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
		CMessage::Create(CMessage::MESSAGE_SUCCESS_SAVE_STAGE);

		return S_OK;
	}
	else {
		//メッセージの画像を生成
		CMessage::Create(CMessage::MESSAGE_BLOCK, D3DCOLOR_RGBA(255, 255, 255, 155), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
		CMessage::Create(CMessage::MESSAGE_MISS_SAVE_STAGE);
	}

	return E_FAIL;
}

HRESULT CCreateStage::CreateStage(void) {
	FILE* fp;
	//モードによって開くファイルを変える
	if (CManager::GetMode() == CManager::MODE_CREATE) {
		fp = fopen(TEXT_NUM, "r");
	}
	D3DXVECTOR3 pos;
	int nType = 0;

	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(CManager::PRIORITY_ENEMY, nCount);
		if (pObject != NULL) {
			if (CManager::GetMode() == CManager::MODE_CREATE) {
				m_bJudge = true;
			}
			if (CManager::GetMode() == CManager::MODE_GAME) {
				CEnemy::UninitSome((CObject::OBJTYPE)CManager::PRIORITY_ENEMY, false);
			}
			break;
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME) {
		m_pPlayer = CManager::GetPlayer();
		//事前に読み込んでいたステージ情報を元にステージを生成する
		for (int nCount = 0; nCount < m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][0].nNum; nCount++) {

			CEnemy::Create(D3DXVECTOR3(m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][nCount].pos.x, 
				                       m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][nCount].pos.y - (SCREEN_HEIGHT / 2), 
				                       m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][nCount].pos.z),
				   CAnimal::ENEMY_TYPE(m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][nCount].Type),
				   CObject::OBJTYPE_ENEMY,
				                       m_aStageDate[m_pPlayer->GetDeathStage(0)][m_pPlayer->GetDeathStage(1)][nCount].pos.y);
		}

		return S_OK;
	}

	if (CManager::GetMode() == CManager::MODE_CREATE) {
		if (fp != NULL) {
			if (m_bJudge == false) {
				//制作画面なら読み込んだステージのみを生成する
				if (CManager::GetMode() == CManager::MODE_CREATE) {
					while (fscanf(fp, "%f %f %f %d", &pos.x, &pos.y, &pos.z, &nType) != EOF) {
						CEnemy::Create(D3DXVECTOR3(pos), (CAnimal::ENEMY_TYPE)nType, CObject::OBJTYPE_ENEMY);
					}
				}
			}
			fclose(fp);
			m_bJudge = false;

			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CCreateStage::LoadStage(void) {
	FILE* fp;
	int nNum = 0;

	if (CManager::GetMode() == CManager::MODE_TITLE) {
		//ステージのデータをすべて読み込む
		for (int nMap = 0; nMap < MAP_MAX; nMap++) {
			for (int nStage = 0; nStage < STAGE_MAX; nStage++) {
				fp = fopen(m_aFileName[nMap][nStage], "r");
				if (fp != NULL) {
					while (fscanf(fp, "%f %f %f %d", &m_aStageDate[nMap][nStage][nNum].pos.x,
						&m_aStageDate[nMap][nStage][nNum].pos.y,
						&m_aStageDate[nMap][nStage][nNum].pos.z,
						&m_aStageDate[nMap][nStage][nNum].Type) != EOF) {
						nNum++;
					}
					m_aStageDate[nMap][nStage][0].nNum = nNum;

					nNum = 0;
				}
				if (fp == NULL) {
					//メッセージの画像を生成
					CMessage::Create(CMessage::MESSAGE_BLOCK, D3DCOLOR_RGBA(255, 255, 255, 155), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
					CMessage::Create(CMessage::MESSAGE_MISS_READ_TXT);
				}
			}
		}
	}

	return S_OK;
}

void CCreateStage::SetEnemy(void) {
	//制作画面で敵を死亡状態->生存状態に変更
	//各数値を初期化
	//総数を増やす
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(CManager::PRIORITY_ENEMY, nCount);
		if (pObject != NULL) {
			if (((CEnemy*)pObject)->m_State == CAnimal::STATE_DEATH) {
				CEnemy::nNumAll++;
			}
			((CEnemy*)pObject)->m_State = CAnimal::STATE_LIFE;
			((CEnemy*)pObject)->SetPOS(((CEnemy*)pObject)->m_fastpos);
			((CEnemy*)pObject)->m_fAngle = 0;
		}
	}
}

bool CCreateStage::GetBoolean(void) {
	return m_bEnemyBox;
}