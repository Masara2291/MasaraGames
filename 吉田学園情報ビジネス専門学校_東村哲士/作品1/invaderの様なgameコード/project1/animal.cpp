#define _CRT_SECURE_NO_WARNINGS

#include "animal.h"
#include "CreateStage.h"
#include "player.h"
#include "boss.h"
#include "explosion.h"
#include "score.h"
#include "message.h"
#include "sound.h"

int CAnimal::m_nFrame = 0;
bool CAnimal::m_bReadJudge = false;

CAnimal::CAnimal(int nPriority) : CScene2d(nPriority) {

}

CAnimal::~CAnimal() {

}

void CAnimal::SetState(STATE Sate) {
	m_State = Sate;
}

CAnimal::STATE CAnimal::GetState(void) {
	return m_State;
}

void CAnimal::SetLife(int nLife) {
	m_nLife = nLife;
}

void CAnimal::HitDamage(int nDamage, CObject* pObject, D3DXVECTOR3 pos) {
	CObject* P_Object;
	int nDeathMap = CPlayer::GetDeathStage(0);
	int nDeathStage = CPlayer::GetDeathStage(1);
	m_pScore = CManager::GetScore();

	m_nLife -= nDamage;

	if (pos.x > FALSE_POS && pos.y > FALSE_POS) {
		CExplosion::Create(pos);
	}

	if (m_nLife <= 0) {
		((CAnimal*)pObject)->m_State = STATE_DEATH;

		if (m_Type == TYPE_ENEMY) {
			m_pScore->AddScore(110);
		}

		if (m_Type == TYPE_PLAYER) {
			if (CManager::GetOldMode() == CManager::MODE_TITLE) {
				CPlayer::SetDeathStage(CPlayer::GetDeathStage(0), CPlayer::GetDeathStage(1));
				m_bReadJudge = false;
				CManager::SetMode(CManager::MODE_RESULT);
			}
			if (CManager::GetOldMode() == CManager::MODE_CREATE) {
				CManager::SetMode(CManager::MODE_TESTRESULT);
			}

			return;
		}

		if (m_Type == TYPE_BOSS) {
			m_pSound = CManager::GetSound();
			m_pSound->StopSound(CSound::SOUND_LABEL_BGM003);
			m_pSound->SoundPlay(CSound::SOUND_LABEL_BGM000);

			m_pScore->AddScore(nDeathMap * 10000 + 10000);

			nDeathStage = 0;

			((CAnimal*)pObject)->Uninit();

			//�}�b�v��1�i�߂�
			nDeathMap++;

			m_bReadJudge = false;

			//�}�b�v�ƃX�e�[�W���o�^�����i�񂾎�
			if (nDeathMap >= CCreateStage::MAP_MAX && nDeathStage >= CCreateStage::STAGE_0) {
				//�e�L�X�g�t�@�C����0���L��
				FILE* fp;
				fp = fopen(DEATH_STAGE_TXT, "w");
				if (fp != NULL) {
					fprintf(fp, "%d", 0);

					fclose(fp);
				}

				CPlayer::SetDeathStage(0, 0);
				//���U���g��ʂɑJ��
				CManager::SetMode(CManager::MODE_RESULT);
				return;
			}

			//�X�e�[�W��1�߂�
			nDeathStage--;

			CPlayer::SetDeathStage(nDeathMap, nDeathStage);

			for (int nCount = 0; nCount < MAX_NUM; nCount++) {
				//�e�̏����擾
				P_Object = CObject::GetScene(CManager::PRIORITY_BULLET, nCount);
				if (P_Object != NULL) {
					((CBullet*)P_Object)->Uninit();
					CExplosion::Create(((CBullet*)P_Object)->GetPos());
				}
			}
		}
	}

	//���̃X�e�[�W�̓G��S�Ď��S��Ԃɂ����Ƃ�
	if (CEnemy::GetNumAll() <= 0 && m_bReadJudge == false && CBoss::GetNumAll() <= 0) {
		//�X�e�[�W��1�i�߂�
		nDeathStage++;

		CPlayer::SetDeathStage(nDeathMap, nDeathStage);

		//�X�e�[�W���o�^�����J�E���g������
		if (nDeathStage > CCreateStage::STAGE_MAX - 1&& CBoss::GetNumAll() <= 0) {
			CMessage::Create(CMessage::MESSAGE_WARNING, 
							 D3DCOLOR_RGBA(255, 255, 255, 0), 
							 D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 0), 
							 D3DXVECTOR3(MESSAGE_SIZ_X * 2, MESSAGE_SIZ_Y * 2, 0));
			m_pSound = CManager::GetSound();
			m_pSound->StopSound(CSound::SOUND_LABEL_BGM000);
			m_bReadJudge = true;
		}
		else if (CBoss::GetNumAll() == 0) {
			//�X�e�[�W��ǂݍ���
			CCreateStage* pCreatStage;
			pCreatStage = new CCreateStage;
			pCreatStage->Init();

			pCreatStage->CreateStage();

			delete pCreatStage;
			pCreatStage = NULL;
		}

		//�}�b�v��1�ڈȊO�̏ꍇ
		if (nDeathMap >= CCreateStage::MAP_0 + 1) {
			//���̃X�e�[�W�̐������e�L�X�g�t�@�C���ɋL��
			FILE* fp;
			fp = fopen(DEATH_STAGE_TXT, "w");
			if (fp != NULL) {
				fprintf(fp, "%d", nDeathMap);
			}

			fclose(fp);
		}

		CPlayer::SetDeathStage(nDeathMap, nDeathStage);
	}

	if (m_bReadJudge == true && CBoss::GetNumAll() <= 0 && CManager::GetMode() == CManager::MODE_GAME && nDeathStage == CCreateStage::STAGE_MAX) {
		CountExplosion(DEFAULT_FRAME_NUM);
	}
}

void CAnimal::SetType(ANIMAL_TYPE Type) {
	m_Type = Type;
}

CAnimal::ANIMAL_TYPE CAnimal::GetType(void) {
	return m_Type;
}

void CAnimal::CountExplosion(int nSecond) {
	m_nFrame++;

	if (m_nFrame <= nSecond * 60) {
		if (m_nFrame == 1) {
			m_pSound = CManager::GetSound();
			m_pSound->SoundPlay(CSound::SOUND_LABEL_WARNING);
		}
		CExplosion::Create(D3DXVECTOR3(GetRand(0, SCREEN_WIDTH), GetRand(0, SCREEN_HEIGHT/ 3), 0),
						   D3DXVECTOR3(EFFECT_SIZ, EFFECT_SIZ, 0));
	}
	else {
		m_pSound = CManager::GetSound();
		m_pSound->StopSound(CSound::SOUND_LABEL_WARNING);
		m_pSound->SoundPlay(CSound::SOUND_LABEL_BGM003);
		//�{�X�𐶐�
		CBoss::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, 0), (BOSS_TYPE)CPlayer::GetDeathStage(0), OBJTYPE_BOSS);
		//�t���[�����̏�����
		m_nFrame = 0;
	}
}

int CAnimal::GetFrame(void) {
	return m_nFrame;
}

void CAnimal::SetReadJudge(bool bJudge) {
	m_bReadJudge = bJudge;
}