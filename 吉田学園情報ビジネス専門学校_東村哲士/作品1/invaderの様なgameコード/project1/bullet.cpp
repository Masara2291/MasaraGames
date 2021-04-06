#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "result.h"
#include "sound.h"
#include "score.h"
#include "math.h"
#include "boss.h"

LPDIRECT3DTEXTURE9 CBullet::m_pTexture[BULLET_TEXTURE_NUM] = {};

CBullet::CBullet(int nPriority) : CScene2d(nPriority) {
	
}

CBullet::~CBullet() {

}

HRESULT CBullet::Init(void) {
	if (m_Type != BULLET_TYPE_GRAVITY) {
		CScene2d::SetTexture(m_pTexture[0]);
	}
	else {
		CScene2d::SetTexture(m_pTexture[1]);
	}
	CScene2d::Init();

	return S_OK;
}

void CBullet::Uninit(void) {
	CScene2d::Uninit();
}

void CBullet::Update(void) {
	D3DXVECTOR3 Judge_pos;
	D3DXVECTOR3 pos = GetPos();
	m_pSound = CManager::GetSound();
	bool bHeifhtJudge = false;

	//’e‚ðˆÚ“®
	pos += m_move;

	//‰æ–ÊŠO”»’è
	if (pos.x + (m_siz.x / 2) <= 0 || pos.x - (m_siz.x / 2) >= SCREEN_WIDTH ||
		pos.y + (m_siz.y / 2) <= 0 || pos.y - (m_siz.y / 2) >= SCREEN_HEIGHT ||
		m_nLife <= 0) {

		if (m_Type == BULLET_TYPE_UP_DOWN && m_move.y <= 0 && m_Type == BULLET_TYPE_INVOLUTE_CURVE && m_fAngle >= 3600 || m_nLife <= 0) {
			Uninit();
		}
		if (m_Type == BULLET_TYPE_UP_DOWN) {
			m_nLife = 2;
			bHeifhtJudge = true;
			pos.y = SCREEN_HEIGHT + (m_siz.y / 2);
		}
		if (m_nLife <= 0 && m_Type != BULLET_TYPE_GRAVITY) {
			CExplosion::Create(pos);
		}
		if (pos.y + (m_siz.y / 2) <= 0 && m_Type != BULLET_TYPE_INVOLUTE_CURVE) {
			m_nLife = 0;
		}
	}
	else {
		m_nLife--;

		//XV
		CScene2d::SetPOS(pos);
		CScene2d::SetSIZ(D3DXVECTOR3(m_siz.x, m_siz.y, 0));
		CScene2d::SetCOL(m_col);
		CScene2d::Update();
	}

	//ã‰º‰^“®’e‚ÌXV
	if (m_Type == BULLET_TYPE_UP_DOWN && bHeifhtJudge == true) {
		m_nFrame++;

		if (m_nFrame == 60) {
			m_move.y *= -1;
			m_nLife = 1000;
		}
	}

	for (int nType = 0; nType < MAX_PRIORITY_NUM; nType++) {
		if (nType == CManager::PRIORITY_PLAYER || nType == CManager::PRIORITY_ENEMY || nType == CManager::PRIORITY_BOSS) {
			for (int nCount = 0; nCount < MAX_NUM; nCount++) {
				CObject* pObject = CObject::GetScene(nType, nCount);

				if (pObject != NULL) {

					OBJTYPE ObjType = CObject::GetObjType();

					//Ž©‹@‚Ì’e‚Æ¶‘¶ó‘Ô‚Ì“G‚Æ‚Ì“–‚½‚è”»’è
					if (pObject->GetObjType() == OBJTYPE_ENEMY && ObjType == OBJTYPE_PLAYER_BULLET && ((CEnemy*)pObject)->m_State == CAnimal::STATE_LIFE) {
						Judge_pos = ((CEnemy*)pObject)->GetPos();

						if (CMath::CollisionDetection(D3DXVECTOR3(pos), D3DXVECTOR3(Judge_pos),
							D3DXVECTOR3(m_siz.x, m_siz.y, 0), D3DXVECTOR3(ENEMY_SIZ, ENEMY_SIZ, 0)) == true) {

							m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_HIT);
							Uninit();
							((CEnemy*)pObject)->HitDamage(1, pObject, Judge_pos);
							((CEnemy*)pObject)->m_State = ((CEnemy*)pObject)->GetState();

							return;
						}
					}

					else if (pObject->GetObjType() == OBJTYPE_BOSS && ObjType == OBJTYPE_PLAYER_BULLET && ((CEnemy*)pObject)->m_State == CAnimal::STATE_LIFE) {
						Judge_pos = ((CEnemy*)pObject)->GetPos();

						if (CMath::CollisionDetection(D3DXVECTOR3(pos), D3DXVECTOR3(Judge_pos),
							D3DXVECTOR3(m_siz.y, m_siz.y, 0), D3DXVECTOR3(BOSS_SIZ, BOSS_SIZ, 0)) == true) {

							m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_HIT);
							Uninit();
							((CBoss*)pObject)->HitDamage(1, pObject, Judge_pos);
							((CBoss*)pObject)->m_State = ((CBoss*)pObject)->GetState();

							return;
						}
					}

					//“G‚Ì’e‚Æ¶‘¶ó‘Ô‚ÌŽ©‹@‚Æ‚Ì“–‚½‚è”»’è
					else if (pObject->GetObjType() == OBJTYPE_PLAYER_DRAW && ObjType == OBJTYPE_ENEMY_BULLET) {
						Judge_pos = ((CPlayer*)pObject)->GetPos();

						//ƒz[ƒ~ƒ“ƒO’e‚ÌŠp“xÄŒvŽZ
						if (m_Type == BULLET_TYPE_INITIAL_TAILGATING) {
							m_move = D3DXVECTOR3(cosf(CScene2d::GetAngle(Judge_pos, pos)) * BULLET_VECTOR / 2, sinf(CScene2d::GetAngle(Judge_pos, pos)) * BULLET_VECTOR / 2, 0);
						}

						if (m_Type != BULLET_TYPE_GRAVITY) {
							if (CMath::CollisionDetection(D3DXVECTOR3(pos), D3DXVECTOR3(Judge_pos),
								D3DXVECTOR3(m_siz.y, m_siz.y, 0), D3DXVECTOR3(PLAYER_JUDGE_SIZ_X, PLAYER_JUDGE_SIZ_Y, 0)) == true) {
								m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_HIT);
								Uninit();
								((CPlayer*)pObject)->HitDamage(1, pObject, Judge_pos);
								((CPlayer*)pObject)->m_State = ((CPlayer*)pObject)->GetState();
							}
						}

						return;
					}
				}
			}
		}
	}
}

void CBullet::Draw(void) {
	CScene2d::Draw();
}

CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, OBJTYPE ObjType, D3DCOLOR col, BULLET_TYPE BulletType, int nLife, D3DXVECTOR3 siz) {
	CBullet* pBullet;
	pBullet = new CBullet(CManager::PRIORITY_BULLET);

	if (pBullet != NULL) {
		pBullet->m_Type = BulletType;
		pBullet->Init();

		pBullet->m_col = col;
		pBullet->SetPOS(pos);
		pBullet->m_move = move;
		pBullet->SetObjType(ObjType);
		pBullet->m_nLife = nLife;
		pBullet->m_siz = siz;
		pBullet->m_nFrame = 0;
		pBullet->m_fAngle = 0;
	}

	return pBullet;
}

HRESULT CBullet::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	//ƒeƒNƒXƒ`ƒƒ“Ç‚Ýž‚Ý
	D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bullet000.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/bullet001.png", &m_pTexture[1]);

	return S_OK;
}

void CBullet::UnLoad(void) {
	for (int nCount = 0; nCount < BULLET_TEXTURE_NUM; nCount++) {
		if (m_pTexture[nCount] != NULL) {
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

CBullet::BULLET_TYPE CBullet::GetBulletType(void) {
	return m_Type;
}