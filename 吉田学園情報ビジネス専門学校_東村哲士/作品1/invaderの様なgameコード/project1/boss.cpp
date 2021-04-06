#include "boss.h"
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "math.h"

LPDIRECT3DTEXTURE9 CBoss::m_pTexture[BOSSTYPE_MAX] = {};	// テクスチャへのポインタ
int CBoss::m_nNumAll = 0;
bool CBoss::m_bJudgeFinsh = false;

CBoss::CBoss(int nPriority) : CAnimal(nPriority) {
	m_nNumAll++;
}

CBoss::~CBoss() {
	m_nNumAll--;
}

HRESULT CBoss::Init(void) {
	CScene2d::Init();

	return S_OK;
}

void CBoss::Uninit(void) {
	CScene2d::Uninit();
}

void CBoss::Update(void) {
	D3DXVECTOR3 pos = CScene2d::GetPos();

	AttackTypeSwitch(pos);

	CScene2d::SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
	CScene2d::Update();
}

void CBoss::AttackTypeSwitch(D3DXVECTOR3 pos) {
	D3DXVECTOR3 Judge_pos;
	m_nFrame++;
	int nAngle;
	int x = 0;
	int y = 0;
	int nVector = 0;

	if (m_nFrame >= ATTACK_INTERVAL_TIME * 60) {
		m_nFrame = 0;
		int nRand = rand() % 3 + 1;
		switch (nRand) {
		case 1:
			for (int nCount = 0; nCount < 1000; nCount++) {
				if (nCount % 100 == 0) {
					x = rand() % SCREEN_WIDTH + 1;
					CBullet::Create(D3DXVECTOR3(x, 0.0f, 0.0f), D3DXVECTOR3(0, BULLET_VECTOR / 2, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(0, 255, 255, 255), CBullet::BULLET_TYPE_NORMAL, 1000);
				}
			}
			break;

		case 2:
			for (int nCount = 0; nCount < 1000; nCount++) {
				if (nCount % 100 == 0) {
					y = rand() % SCREEN_HEIGHT + 1;
					nVector = rand() % 2 + 1;
					if (nVector == 1) {
						CBullet::Create(D3DXVECTOR3(SCREEN_WIDTH, y, 0.0f), D3DXVECTOR3(-(BULLET_VECTOR / 2), 0, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(0, 255, 255, 255), CBullet::BULLET_TYPE_NORMAL, 1000);
					}
					if (nVector == 2) {
						CBullet::Create(D3DXVECTOR3(0, y, 0.0f), D3DXVECTOR3(BULLET_VECTOR / 2, 0, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(0, 255, 255, 255), CBullet::BULLET_TYPE_NORMAL, 1000);
					}
				}
			}
			break;

		case 3:
			m_bJudgeFinsh = true;
			m_bUseJudge = false;

			break;
		}
	}

	if (m_bJudgeFinsh == true) {

		switch (m_Type) {
			if (m_bUseJudge == false) {
		case BOSS_TYPE_0:
			CBullet::Create(D3DXVECTOR3(GetRand(0 + (GRAVITY_SIZ / 2), SCREEN_WIDTH - (GRAVITY_SIZ / 2)), SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 0.0f), D3DXVECTOR3(0, 0, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 255, 255, 60), CBullet::BULLET_TYPE_GRAVITY, BULLET_LIFE * 10, D3DXVECTOR3(GRAVITY_SIZ, GRAVITY_SIZ, 0));
			break;

		case BOSS_TYPE_1:
			nAngle = GetRand(0, 360);
			for (int nCount = 0; nCount < 360; nCount++) {
				if (nCount % 36 == 0) {
					CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
						D3DXVECTOR3(cosf((nCount + nAngle) * D3DX_PI / 180.0f), sinf((nCount + nAngle) * D3DX_PI / 180.0f), 0),
						OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 0, 255, 255),
						CBullet::BULLET_TYPE_INVOLUTE_CURVE,
						1000);
				}
			}
			break;

		case BOSS_TYPE_2:
			for (int nCount = 0; nCount < 10; nCount++) {
				x = rand() % SCREEN_WIDTH + 1;
				CBullet::Create(D3DXVECTOR3(x, 0.0f, 0.0f), D3DXVECTOR3(0, BULLET_VECTOR / 2, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 0, 255, 255), CBullet::BULLET_TYPE_UP_DOWN, 1000);
			}
			break;

		case BOSS_TYPE_3:
			int nRand = rand() % 3 + 1;
			switch (nRand) {
			case 1:
				CBullet::Create(D3DXVECTOR3(GetRand(0 + (GRAVITY_SIZ / 2), SCREEN_WIDTH - (GRAVITY_SIZ / 2)), SCREEN_HEIGHT - (SCREEN_HEIGHT / 4), 0.0f), D3DXVECTOR3(0, 0, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 255, 255, 60), CBullet::BULLET_TYPE_GRAVITY, BULLET_LIFE * 10, D3DXVECTOR3(GRAVITY_SIZ, GRAVITY_SIZ, 0));
				break;

			case 2:
				nAngle = GetRand(0, 360);
				for (int nCount = 0; nCount < 360; nCount++) {
					if (nCount % 36 == 0) {
						CBullet::Create(D3DXVECTOR3(pos.x, pos.y, 0.0f),
							D3DXVECTOR3(cosf((nCount + nAngle) * D3DX_PI / 180.0f), sinf((nCount + nAngle) * D3DX_PI / 180.0f), 0),
							OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(255, 0, 255, 255),
							CBullet::BULLET_TYPE_INVOLUTE_CURVE,
							1000);
					}
				}
				break;

			case 3:
				for (int nCount = 0; nCount < 10; nCount++) {
					x = rand() % SCREEN_WIDTH + 1;
					CBullet::Create(D3DXVECTOR3(x, 0.0f, 0.0f), D3DXVECTOR3(0, BULLET_VECTOR / 2, 0), OBJTYPE_ENEMY_BULLET, D3DCOLOR_RGBA(153, 0, 255, 255), CBullet::BULLET_TYPE_UP_DOWN, 1000);
				}
				break;
			}

			break;
			}
		}

		m_bJudgeFinsh = false;
		m_bUseJudge = true;
	}
}

D3DXVECTOR3 CBoss::MoveTypeSwitch(D3DXVECTOR3 pos, D3DXVECTOR3 Playerpos) {


	return pos;
}

void CBoss::Draw(void) {
	CScene2d::Draw();
}

CBoss* CBoss::Create(D3DXVECTOR3 pos, BOSS_TYPE Type, OBJTYPE ObjType) {
	CBoss* pBoss;
	pBoss = new CBoss(CManager::PRIORITY_BOSS);

	if (pBoss != NULL) {
		pBoss->Init();
		pBoss->SetTexture(m_pTexture[Type]);
		pBoss->m_Type = Type;
		pBoss->SetObjType(ObjType);
		pBoss->SetPOS(pos);
		pBoss->SetSIZ(D3DXVECTOR3(BOSS_SIZ, BOSS_SIZ, 0));
		pBoss->m_State = STATE_LIFE;
		pBoss->SetState(pBoss->m_State);
		pBoss->SetLife(150);
		pBoss->CAnimal::SetType(TYPE_BOSS);
		pBoss->m_bJudgeFinsh = false;
		pBoss->m_nFrame = 0;
		pBoss->m_nAngle = 0;
	}

	return pBoss;
}

void CBoss::SetType(BOSS_TYPE Type) {
	m_Type = Type;
}

CAnimal::BOSS_TYPE CBoss::GetEnemyType(void) {
	return m_Type;
}

int CBoss::GetNumAll(void) {
	return m_nNumAll;
}

HRESULT CBoss::Load(void) {
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

	return S_OK;
}

void CBoss::UnLoad(void) {
	for (int nCount = 0; nCount < BOSSTYPE_MAX; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

void CBoss::SetJudgeFinsh(bool bJudge) {
	m_bJudgeFinsh = bJudge;
}