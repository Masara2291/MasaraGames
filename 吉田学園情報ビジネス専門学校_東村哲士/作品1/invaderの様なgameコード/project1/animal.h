#ifndef ANIMAL_H
#define ANIMAL_H

#include "main.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

class CScore;
class CSound;

#define DEFAULT_FRAME_NUM (3)
#define FALSE_POS (-1)
#define FALSE_SIZ (0)
#define BASE_LIFE (1)
#define DEATH_STAGE_TXT "stage/DeathStage.txt"

class CAnimal : public CScene2d {
public:
	typedef enum {
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BOSS,
		ANIMALTYPE_MAX
	}ANIMAL_TYPE;

	typedef enum {
		BOSS_TYPE_0,
		BOSS_TYPE_1,
		BOSS_TYPE_2,
		BOSS_TYPE_3,
		BOSSTYPE_MAX
	}BOSS_TYPE;

	typedef enum {
		ENEMY_TYPE_0 = 0,
		ENEMY_TYPE_1,
		ENEMY_TYPE_2,
		ENEMY_TYPE_3,
		ERASER_0,
		ENEMYTYPE_MAX
	}ENEMY_TYPE;

	typedef enum {
		STATE_NONE,
		STATE_LIFE,
		STATE_DEATH
	}STATE;

	CAnimal(int nPriority = 3);
	~CAnimal();

	void SetLife(int nLife = BASE_LIFE);
	void HitDamage(int nDamage, CObject* pObject, D3DXVECTOR3 pos = D3DXVECTOR3(FALSE_POS, FALSE_POS, 0));
	void SetState(STATE Sate);
	STATE GetState(void);
	void SetType(ANIMAL_TYPE Type);
	ANIMAL_TYPE GetType(void);
	void CountExplosion(int nSecond = DEFAULT_FRAME_NUM);
	static int GetFrame(void);
	static void SetReadJudge(bool bJudge);

	STATE m_State;
	int m_nLife;
	static int m_nFrame;
	static bool m_bReadJudge;

private:
	ANIMAL_TYPE m_Type;
	CScore* m_pScore;
	CSound* m_pSound;
};

#endif