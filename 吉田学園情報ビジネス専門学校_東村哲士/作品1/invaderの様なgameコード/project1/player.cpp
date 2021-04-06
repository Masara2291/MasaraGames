#define _CRT_SECURE_NO_WARNINGS

#include "player.h"
#include "enemy.h"
#include "explosion.h"
#include "sound.h"
#include "CreateStage.h"
#include "score.h"
#include "math.h"
#include "bullet.h"
#include "boss.h"

LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;
int CPlayer::m_DeathStage[DEATH_STAGE] = {};
D3DXVECTOR3 CPlayer::m_move = D3DXVECTOR3(MOTIVE_POWER, MOTIVE_POWER, 0);

CPlayer::CPlayer(int nPriority) : CAnimal(nPriority) {
	m_DeathStage[0] = 0;
	m_DeathStage[1] = 0;

	FILE* fp;
	fp = fopen(DEATH_STAGE_TXT, "r");
	if (fp != NULL) {
		fscanf(fp, "%d", &m_DeathStage[0]);

		fclose(fp);
	}
	if (fp == NULL) {
		m_DeathStage[0] = 0;
	}
}

CPlayer::~CPlayer() {

}

HRESULT CPlayer::Init(void) {
	CScene2d::Init();

	return S_OK;
}

void CPlayer::Uninit(void) {
	CScene2d::Uninit();
}

void CPlayer::Update(void) {
	D3DXVECTOR3 Judge_pos;
	D3DXVECTOR3 pos = CScene2d::GetPos();
	m_pSound = CManager::GetSound();
	m_pScore = CManager::GetScore();
	bool GravityJudge = false;

	//回転処理
	fAngle += 0.5f;
	fRd = fAngle * D3DX_PI / 180.0f;

	if (fAngle >= 360.0f || fAngle <= -360.0f) {
		fAngle = 0;
	}

	//制作画面以外の時に処理
	if (CManager::GetMode() != CManager::MODE_CREATE) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			//敵の情報を取得
			CObject* pObject = CObject::GetScene(CManager::PRIORITY_ENEMY, nCount);
			if (pObject != NULL) {
				OBJTYPE ObjType = pObject->GetObjType();
				//ステージ上の敵かつプレイヤーが生存状態の時
				if (ObjType == OBJTYPE_ENEMY && ((CEnemy*)pObject)->m_State == STATE_LIFE) {
					Judge_pos = ((CEnemy*)pObject)->GetPos();

					//当たり判定計算
					if (CMath::CollisionDetection(D3DXVECTOR3(pos), D3DXVECTOR3(Judge_pos),
						D3DXVECTOR3(PLAYER_JUDGE_SIZ_X, PLAYER_JUDGE_SIZ_Y, 0), D3DXVECTOR3(ENEMY_SIZ, ENEMY_SIZ, 0)) == true) {

						//プレイヤーを死亡状態に移行させる
						CAnimal::HitDamage(1, CObject::GetScene(CManager::PRIORITY_PLAYER, 0), pos);

						return;
					}
				}
			}

			//ボスの情報を取得
			pObject = CObject::GetScene(CManager::PRIORITY_BOSS, nCount);
			if (pObject != NULL) {
				OBJTYPE ObjType = pObject->GetObjType();
				//ステージ上の敵かつプレイヤーが生存状態の時
				if (ObjType == OBJTYPE_BOSS && ((CEnemy*)pObject)->m_State == STATE_LIFE) {
					Judge_pos = ((CEnemy*)pObject)->GetPos();

					//当たり判定計算
					if (CMath::CollisionDetection(D3DXVECTOR3(pos), D3DXVECTOR3(Judge_pos),
						D3DXVECTOR3(PLAYER_JUDGE_SIZ_X, PLAYER_JUDGE_SIZ_Y, 0), D3DXVECTOR3(BOSS_SIZ, BOSS_SIZ, 0)) == true) {

						//プレイヤーを死亡状態に移行させる
						CAnimal::HitDamage(1, CObject::GetScene(CManager::PRIORITY_PLAYER, 0), pos);
						SetReadJudge(false);

						return;
					}
				}
			}

			//弾の情報を取得
			pObject = CObject::GetScene(CManager::PRIORITY_BULLET, nCount);
			if (pObject != NULL && ((CBullet*)pObject)->m_Type == CBullet::BULLET_TYPE_GRAVITY) {
				Judge_pos = ((CBullet*)pObject)->GetPos();

				if (CMath::CircularityJudgment(D3DXVECTOR3(PLAYER_JUDGE_SIZ_X, PLAYER_JUDGE_SIZ_Y, 0), D3DXVECTOR3(GRAVITY_SIZ, GRAVITY_SIZ, 0),
					D3DXVECTOR3(FALSE_MOVEMENT, FALSE_MOVEMENT, 0), D3DXVECTOR3(Judge_pos)) == true) {

					GravityJudge = true;
					m_move = D3DXVECTOR3(MOTIVE_POWER / 2, MOTIVE_POWER / 2, 0);
				}
			}
		}


		//プレイヤーが死亡状態になった時、リザルト画面に遷移する
		if (m_State == STATE_DEATH) {
			//ステージを0からにする
			m_DeathStage[1] = 0;

			//1つ前のモードによってリザルトを切り替える
			if (CManager::GetOldMode() == CManager::MODE_TITLE || CManager::GetOldMode() == CManager::MODE_RESULT) {
				CManager::SetMode(CManager::MODE_RESULT);
			}

			if (CManager::GetOldMode() == CManager::MODE_CREATE || CManager::GetOldMode() == CManager::MODE_TESTRESULT) {
				CManager::SetMode(CManager::MODE_TESTRESULT);
			}

			return;
		}
	}

	if (GravityJudge == false) {
		m_move = D3DXVECTOR3(MOTIVE_POWER, MOTIVE_POWER, 0);
	}

#ifdef MOVE_JUDGE
	m_pos += PolygonMove;
#endif

#if false
	//回転と大きさ変更//

	D3DXVECTOR3 SIZ = D3DXVECTOR3(sin(fRd) * _POLYGON_SIZ_X, sin(fRd) * _POLYGON_SIZ_Y, 0);

	//回転
	fAngleX0 = float((-(SIZ.x / 2)) * cos(fRd) - (-(SIZ.y / 2)) * sin(fRd));
	fAngleY0 = float((-(SIZ.x / 2)) * sin(fRd) + (-(SIZ.y / 2)) * cos(fRd));
	fAngleX1 = float((+(SIZ.x / 2)) * cos(fRd) - (-(SIZ.y / 2)) * sin(fRd));
	fAngleY1 = float((+(SIZ.x / 2)) * sin(fRd) + (-(SIZ.y / 2)) * cos(fRd));
	fAngleX2 = float((-(SIZ.x / 2)) * cos(fRd) - (+(SIZ.y / 2)) * sin(fRd));
	fAngleY2 = float((-(SIZ.x / 2)) * sin(fRd) + (+(SIZ.y / 2)) * cos(fRd));
	fAngleX3 = float((+(SIZ.x / 2)) * cos(fRd) - (+(SIZ.y / 2)) * sin(fRd));
	fAngleY3 = float((+(SIZ.x / 2)) * sin(fRd) + (+(SIZ.y / 2)) * cos(fRd));

	if (m_pos.x - (SIZ.x / 2) < 0 || m_pos.x + (SIZ.x / 2) > SCREEN_WIDTH) {
		PolygonMove.x *= -1;

#ifdef CHANGE_COL
		pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
		pVtx[1].col = pVtx[0].col;
		pVtx[2].col = pVtx[0].col;
		pVtx[3].col = pVtx[0].col;
#endif
	}
	if (m_pos.y - (SIZ.y / 2) < 0 || m_pos.y + (SIZ.y / 2) > SCREEN_HEIGHT) {
		PolygonMove.y *= -1;

#ifdef CHANGE_COL
		pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
		pVtx[1].col = pVtx[0].col;
		pVtx[2].col = pVtx[0].col;
		pVtx[3].col = pVtx[0].col;
#endif
	}

	pVtx[0].pos = D3DXVECTOR3(m_pos.x + fAngleX0, m_pos.y + fAngleY0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fAngleX1, m_pos.y + fAngleY1, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + fAngleX2, m_pos.y + fAngleY2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fAngleX3, m_pos.y + fAngleY3, 0.0f);
#elif false
	//回転//

	//回転
	fAngleX0 = float((-(_POLYGON_SIZ_X / 2)) * cos(fRd) - (-(_POLYGON_SIZ_Y / 2)) * sin(fRd));
	fAngleY0 = float((-(_POLYGON_SIZ_X / 2)) * sin(fRd) + (-(_POLYGON_SIZ_Y / 2)) * cos(fRd));
	fAngleX1 = float((+(_POLYGON_SIZ_X / 2)) * cos(fRd) - (-(_POLYGON_SIZ_Y / 2)) * sin(fRd));
	fAngleY1 = float((+(_POLYGON_SIZ_X / 2)) * sin(fRd) + (-(_POLYGON_SIZ_Y / 2)) * cos(fRd));
	fAngleX2 = float((-(_POLYGON_SIZ_X / 2)) * cos(fRd) - (+(_POLYGON_SIZ_Y / 2)) * sin(fRd));
	fAngleY2 = float((-(_POLYGON_SIZ_X / 2)) * sin(fRd) + (+(_POLYGON_SIZ_Y / 2)) * cos(fRd));
	fAngleX3 = float((+(_POLYGON_SIZ_X / 2)) * cos(fRd) - (+(_POLYGON_SIZ_Y / 2)) * sin(fRd));
	fAngleY3 = float((+(_POLYGON_SIZ_X / 2)) * sin(fRd) + (+(_POLYGON_SIZ_Y / 2)) * cos(fRd));

	if (m_pos.x - (_POLYGON_SIZ_X / 2) <= 0 || m_pos.x + (_POLYGON_SIZ_X / 2) >= SCREEN_WIDTH) {
		PolygonMove.x *= -1;

#ifdef CHANGE_COL
		pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
		pVtx[1].col = pVtx[0].col;
		pVtx[2].col = pVtx[0].col;
		pVtx[3].col = pVtx[0].col;
#endif
	}
	if (m_pos.y - (_POLYGON_SIZ_Y / 2) <= 0 || m_pos.y + (_POLYGON_SIZ_Y / 2) >= SCREEN_HEIGHT) {
		PolygonMove.y *= -1;

#ifdef CHANGE_COL
		pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
		pVtx[1].col = pVtx[0].col;
		pVtx[2].col = pVtx[0].col;
		pVtx[3].col = pVtx[0].col;
#endif
	}

	pVtx[0].pos = D3DXVECTOR3(m_pos.x + fAngleX0, m_pos.y + fAngleY0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fAngleX1, m_pos.y + fAngleY1, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + fAngleX2, m_pos.y + fAngleY2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fAngleX3, m_pos.y + fAngleY3, 0.0f);
#elif true
	if (CManager::GetMode() != CManager::MODE_CREATE) {
		//操作//
		m_pInput = CManager::GetKeybord();

		if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_W)) {
			if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_A)) {
				pos.x -= (m_move.x / 5) * cosf(45);
				pos.y -= (m_move.y / 5) * sinf(45);
			}
			if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_D)) {
				pos.x += (m_move.x / 5) * cosf(45);
				pos.y -= (m_move.y / 5) * sinf(45);
			}
			pos.y -= m_move.y;
		}
		if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_S)) {
			if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_A)) {
				pos.x -= (m_move.x / 5) * cosf(45);
				pos.y += (m_move.y / 5) * sinf(45);
			}
			if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_D)) {
				pos.x += (m_move.x / 5) * cosf(45);
				pos.y += (m_move.y / 5) * sinf(45);
			}
			pos.y += m_move.y;
		}
		if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_A)) {
			pos.x -= m_move.x;
		}
		if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_D)) {
			pos.x += m_move.x;
		}

		if (((CKeybord*)m_pInput)->GetkeyboardPress(DIK_SPACE) && CObject::GetObjType() == OBJTYPE_PLAYER_DRAW) {
			if (m_nCount % ATTACK_INTERVAL == 0) {
				m_pSound->SoundPlay(CSound::SOUND_LABEL_SE_SHOT);
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y - (PLAYER_JUDGE_SIZ_Y / 2), pos.z), D3DXVECTOR3(0, -BULLET_VECTOR, 0), OBJTYPE_PLAYER_BULLET, D3DCOLOR_RGBA(255, 255, 255, 255));
				if (CScore::GetScore() > 0 && CBoss::GetNumAll() < 1) {
					m_pScore->AddScore(-10);
				}
			}
			m_nCount += 1;
		}
		if (((CKeybord*)m_pInput)->GetkeyboardRelease(DIK_SPACE) && CObject::GetObjType() == OBJTYPE_PLAYER_DRAW) {
			m_nCount = ATTACK_INTERVAL;
		}


		//毎フレームプレイヤーに0ダメージ与え、HitDamage関数を呼び出す
		HitDamage(0, GetScene(CManager::PRIORITY_PLAYER, 0));


#ifdef MOVE_JUDGE
		if (m_pos.x - (_POLYGON_SIZ_X / 2) <= 0 || m_pos.x + (_POLYGON_SIZ_X / 2) >= SCREEN_WIDTH) {
			PolygonMove.x *= -1;

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
		if (m_pos.y - (_POLYGON_SIZ_Y / 2) <= 0 || m_pos.y + (_POLYGON_SIZ_Y / 2) >= SCREEN_HEIGHT) {
			PolygonMove.y *= -1;

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
#endif
#ifndef MOVE_JUDGE
		if (pos.x - (PLAYER_JUDGE_SIZ_X / 2) <= 0) {
			pos.x = 0 + (PLAYER_JUDGE_SIZ_X / 2);

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
		if (pos.x + (PLAYER_JUDGE_SIZ_X / 2) >= SCREEN_WIDTH) {
			pos.x = SCREEN_WIDTH - (PLAYER_JUDGE_SIZ_X / 2);

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
		if (pos.y - (PLAYER_JUDGE_SIZ_Y / 2) <= 0) {
			pos.y = 0 + (PLAYER_JUDGE_SIZ_Y / 2);

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
		if (pos.y + (PLAYER_JUDGE_SIZ_Y / 2) >= SCREEN_HEIGHT) {
			pos.y = SCREEN_HEIGHT - (PLAYER_JUDGE_SIZ_Y / 2);

#ifdef CHANGE_COL
			pVtx[0].col = D3DCOLOR_RGBA(rand() % 256, rand() % 256, rand() % 256, 255);
			pVtx[1].col = pVtx[0].col;
			pVtx[2].col = pVtx[0].col;
			pVtx[3].col = pVtx[0].col;
#endif
		}
#endif

	}

	CScene2d::SetPOS(pos);
	CScene2d::SetSIZ(D3DXVECTOR3(PLAYER_DRAW_SIZ_X, PLAYER_DRAW_SIZ_X, 0));
	CScene2d::SetCOL(D3DCOLOR_RGBA(255, 255, 255, 255));
	CScene2d::Update();
#endif
}

void CPlayer::Draw(void) {
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(CManager::PRIORITY_PLAYER, nCount);

		if (pObject != NULL) {
			OBJTYPE ObjType = pObject->GetObjType();
			if (ObjType == OBJTYPE_PLAYER_DRAW) {
				CScene2d::Draw();
				break;
			}
		}
	}
}

CPlayer* CPlayer::Create(D3DXVECTOR3 pos, OBJTYPE ObjType) {
	CPlayer* pPlayer;
	pPlayer = new CPlayer(CManager::PRIORITY_PLAYER);
	
	if (pPlayer != NULL) {
		pPlayer->Init();
		pPlayer->SetObjType(ObjType);
		pPlayer->SetTexture(m_pTexture);
		pPlayer->SetPOS(pos);
		pPlayer->m_State = STATE_LIFE;
		pPlayer->SetLife(1);
		pPlayer->SetType(TYPE_PLAYER);
		pPlayer->m_move = D3DXVECTOR3(MOTIVE_POWER, MOTIVE_POWER, 0);
		pPlayer->m_nCount = ATTACK_INTERVAL;
	}

	return pPlayer;
}

HRESULT CPlayer::Load(void) {
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_pTexture == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/player001.png", &m_pTexture);

		return S_OK;
	}

	return E_FAIL;
}

void CPlayer::UnLoad(void) {
	if (m_pTexture != NULL) {
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

int CPlayer::GetDeathStage(int nNum) {
	return m_DeathStage[nNum];
}

void CPlayer::SetDeathStage(int nNum1, int nNum2) {
	m_DeathStage[0] = nNum1;
	m_DeathStage[1] = nNum2;
}

void CPlayer::SetMove(D3DXVECTOR3 move) {
	m_move = move;
}