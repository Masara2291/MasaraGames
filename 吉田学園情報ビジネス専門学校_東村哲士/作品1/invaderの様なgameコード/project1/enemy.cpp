#include "enemy.h"
#include "bullet.h"
#include "player.h"
#include "sound.h"
#include "CreateStage.h"
#include "math.h"

LPDIRECT3DTEXTURE9 CEnemy::m_pTexture[ENEMYTYPE_MAX] = {};
int CEnemy::nNumAll = NULL;
int CEnemy::m_nCreatEnemyNum = NULL;
bool CEnemy::m_bCreatEnemy = false;

CEnemy::CEnemy(int nPriority) : CAnimal(nPriority) {
	nNumAll++;
}

CEnemy::~CEnemy()
{
	//ここに「nNumAll--;」を書かない！
}

HRESULT CEnemy::Init(void)
{
	CScene2d::Init();

	return S_OK;
}

void CEnemy::Uninit(void)
{
	CScene2d::Uninit();
}

void CEnemy::Update(void)
{
	D3DXVECTOR3 pos = CScene2d::GetPos();

	//ゲームモードが制作画面以外かつ、敵がステージ上にいる敵の時かつ、生存状態かの確認
	if (CManager::GetMode() != CManager::MODE_CREATE && CObject::GetObjType() == CObject::OBJTYPE_ENEMY && m_State == STATE_LIFE) {
		m_pSound = CManager::GetSound();

		//プレイヤーの情報を取得
		CObject* pObject = CObject::GetScene(CManager::PRIORITY_PLAYER, 0);
		//角度を計算
		if (m_Type != CAnimal::ENEMY_TYPE_0 && m_Type != CAnimal::ENEMY_TYPE_3) {
			//プレイヤーと敵との角度を取得
			m_fAngle = CScene2d::GetAngle(((CPlayer*)pObject)->GetPos(), pos);
		}
		if (m_Type == CAnimal::ENEMY_TYPE_0 || m_Type == CAnimal::ENEMY_TYPE_3) {
			//0～360°まで0.04ずつ変化させる
			//加算値によって速度が変化する
			m_fAngle += 0.04;
			//角度が360以上または-360以下になった場合0に戻す
			if (m_fAngle >= 360 || m_fAngle <= -360) {
				m_fAngle = 0;
			}
		}

		if (pos.y < 0) {
			m_bScreen_Judgment = false;
		}
		if (pos.x - (ENEMY_SIZ / 2) >= 0 &&
			pos.x + (ENEMY_SIZ / 2) <= SCREEN_WIDTH &&
			pos.y - (ENEMY_SIZ / 2) >= 0 &&
			pos.y + (ENEMY_SIZ / 2) <= SCREEN_HEIGHT) {
			m_bScreen_Judgment = true;
		}

		//敵のタイプがTYPE_Mental_Health以外の場合
		if (m_Type != CAnimal::ENEMY_TYPE_2) {
			if (m_bScreen_Judgment == true) {
				//画面外判定X
				if (pos.x - (ENEMY_SIZ / 2) <= 0 || pos.x + (ENEMY_SIZ / 2) >= SCREEN_WIDTH) {
					m_move.x *= -1;
				}
				//画面外判定Y
				if (pos.y - (ENEMY_SIZ / 2) <= 0 || pos.y + (ENEMY_SIZ / 2) >= SCREEN_HEIGHT) {
					m_move.y *= -1;
				}
			}
		}
		//敵のタイプがTYPE_Mental_Healthの場合
		if (m_Type == CAnimal::ENEMY_TYPE_2) {
			if (m_bScreen_Judgment == true) {
				//画面外判定X
				if (pos.x - (ENEMY_SIZ / 2) <= 0) {
					pos.x = 0 + (ENEMY_SIZ / 2);
				}
				if (pos.x + (ENEMY_SIZ / 2) >= SCREEN_WIDTH) {
					pos.x = SCREEN_WIDTH - (ENEMY_SIZ / 2);
				}
				//画面外判定Y
				if (pos.y - (ENEMY_SIZ / 2) <= 0) {
					pos.y = 0 + (ENEMY_SIZ / 2);
				}
				if (pos.y + (ENEMY_SIZ / 2) >= SCREEN_HEIGHT) {
					pos.y = SCREEN_HEIGHT - (ENEMY_SIZ / 2);
				}
			}
		}

		//動き用関数
		pos = MoveTypeSwitch(pos, ((CPlayer*)pObject)->GetPos());
		if (rand() % + 100 <= 4) {
			m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_SHOT);
			//攻撃用関数
			AttackTypeSwitch(pos);
		}
	}

	CScene2d::SetPOS(pos);
	CScene2d::SetSIZ(D3DXVECTOR3(ENEMY_SIZ, ENEMY_SIZ, 0));
	CScene2d::SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
	CScene2d::Update();
}

void CEnemy::Draw(void)
{
	//ゲームモードが制作画面かつタブを使ってる場合か、敵がステージ上の物かつ生存の場合描画する
	if (CObject::GetObjType() == CObject::OBJTYPE_ENEMY_CREAT && CCreateStage::GetBoolean() == true || CObject::GetObjType() == CObject::OBJTYPE_ENEMY && m_State == STATE_LIFE) {
		CScene2d::Draw();
	}
}

void CEnemy::AttackTypeSwitch(D3DXVECTOR3 pos) {
	//各敵タイプによって攻撃パターンを変えている
	if (m_Type == CAnimal::ENEMY_TYPE_0) {
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y + (ENEMY_SIZ / 2), pos.z), D3DXVECTOR3(0, BULLET_VECTOR, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 0, 0, 255));
	}
	if (m_Type == CAnimal::ENEMY_TYPE_1) {
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(cosf(m_fAngle) * BULLET_VECTOR, sinf(m_fAngle) * BULLET_VECTOR, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(0, 0, 255, 255));
	}
	if (m_Type == CAnimal::ENEMY_TYPE_2) {
		CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(cosf(m_fAngle) * (BULLET_VECTOR * 1.5f), sinf(m_fAngle) * (BULLET_VECTOR * 1.5f), 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(0, 255, 0, 255), CBullet::BULLET_TYPE_INITIAL_TAILGATING, BULLET_LIFE * 3);
	}
	if (m_Type == CAnimal::ENEMY_TYPE_3) {
		for (float fAngle = ANGLE_DISCREPANCY; fAngle <= (360.0f - ANGLE_DISCREPANCY); fAngle += (float)(360 / 3)) {
			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(cosf(fAngle) * (BULLET_VECTOR / 2), sinf(fAngle) * (BULLET_VECTOR / 2), 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 255, 0, 255));
		}
	}
}

D3DXVECTOR3 CEnemy::MoveTypeSwitch(D3DXVECTOR3 pos, D3DXVECTOR3 Playerpos) {
	//各敵タイプによって行動パターンを変えている
	if (m_Type == CAnimal::ENEMY_TYPE_0) {
		pos.x += (m_move.x * 2) * cosf(m_fAngle);
	}
	if (m_Type == CAnimal::ENEMY_TYPE_1) {
		pos.x += cosf(m_fAngle) * m_move.x / 2;
		pos.y += sinf(m_fAngle) * m_move.y / 2;
	}
	if (m_Type == CAnimal::ENEMY_TYPE_2) {
		if (abs(Playerpos.x - pos.x) < 200) {
			pos.x -= cosf(m_fAngle) * m_move.x / 1.5f;
		}
		if (abs(Playerpos.y - pos.y) < 200) {
			pos.y -= sinf(m_fAngle) * m_move.y / 1.5f;
		}
		if (abs(Playerpos.x - pos.x) > 200) {
			pos.x += cosf(m_fAngle) * m_move.x / 1.5f;
		}
		if (abs(Playerpos.y - pos.y) > 200) {
			pos.y += sinf(m_fAngle) * m_move.y / 1.5f;
		}
	}
	if (m_Type == CAnimal::ENEMY_TYPE_3) {
		//画面外かでY軸移動を変更
		if (m_bScreen_Judgment == false) {
			pos.y += (ENEMY_VECTOR / 2);
		}
		if (m_bScreen_Judgment == true) {
			pos.y += (m_move.y) * sinf(m_fAngle);
		}

		pos.x += (m_move.x) * cosf(m_fAngle);
	}

	if (m_Type == CAnimal::ENEMY_TYPE_0 || m_Type == CAnimal::ENEMY_TYPE_3 || m_bScreen_Judgment == false) {
		if (pos.y <= m_nStandardY) {
			pos.y += (ENEMY_VECTOR / 2);
		}
		if (pos.y >= m_nStandardY) {
			pos.y -= (ENEMY_VECTOR / 2);
		}
	}

	return pos;
}

CEnemy* CEnemy::Create(D3DXVECTOR3 pos, ENEMY_TYPE Type, OBJTYPE ObjType, int nStandardPosY)
{
	CEnemy* pEnemy;
	//タブに表示する用の敵かをカウントと敵のオブジェクトタイプから分岐させている
	if (m_nCreatEnemyNum < 5 && CManager::GetMode() == CManager::MODE_CREATE || m_nCreatEnemyNum < 5 && CManager::GetOldMode() == CManager::MODE_TESTRESULT) {
		pEnemy = new CEnemy(CManager::PRIORITY_ENEMY_CREAT);
		m_nCreatEnemyNum++;
	}
	else if (CManager::GetOldMode() == CManager::MODE_TITLE || CManager::GetOldMode() == CManager::MODE_TESTRESULT || CManager::GetMode() == CManager::MODE_CREATE) {
		pEnemy = new CEnemy(CManager::PRIORITY_ENEMY);
	}

	if (pEnemy != NULL) {
		pEnemy->Init();
		pEnemy->SetTexture(m_pTexture[Type]);
		pEnemy->SetObjType(ObjType);
		pEnemy->SetPOS(pos);
		pEnemy->m_fastpos = pos;
		pEnemy->SetSIZ(D3DXVECTOR3(ENEMY_SIZ, ENEMY_SIZ, 0));
		pEnemy->m_Type = Type;
		pEnemy->m_fAngle = 0;
		pEnemy->m_State = STATE_LIFE;
		pEnemy->SetState(pEnemy->m_State);
		pEnemy->m_nStandardY = nStandardPosY;
		pEnemy->SetLife(1);
		pEnemy->CAnimal::SetType(CAnimal::TYPE_ENEMY);

		m_bCreatEnemy = true;
	}

	//タブ用の敵だった場合、総数にカウントしないため、総数を数える変数を-1している
	if (ObjType == CObject::OBJTYPE_ENEMY_CREAT) {
		nNumAll--;
	}

	return pEnemy;
}

HRESULT CEnemy::Load(void)
{
	CRenderere* pRenderere = CManager::GetRenderere();

	if (m_pTexture[0] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/enemy000.png", &m_pTexture[0]);
	}
	if (m_pTexture[1] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/enemy001.png", &m_pTexture[1]);
	}
	if (m_pTexture[2] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/enemy002.png", &m_pTexture[2]);
	}
	if (m_pTexture[3] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/enemy003.png", &m_pTexture[3]);
	}
	if (m_pTexture[4] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/eraser000.png", &m_pTexture[4]);
	}

	return S_OK;
}

void CEnemy::UnLoad(void)
{
	for (int nCount = 0; nCount < CAnimal::ENEMYTYPE_MAX; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

int CEnemy::GetNumAll(void) {
	int nNum = 0;

	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(CManager::PRIORITY_ENEMY, nCount);
		if (pObject != NULL && ((CEnemy*)pObject)->m_State == STATE_LIFE) {
			nNum++;
		}
	}

	return nNum;
}

void CEnemy::AddNumAll(int nNum) {
	nNumAll += nNum;
}

void CEnemy::SetType(ENEMY_TYPE Type) {
	m_Type = Type;
}

CEnemy::ENEMY_TYPE CEnemy::GetEnemyType(void) {
	return m_Type;
}