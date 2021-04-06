#ifndef BOSS_H
#define BOSS_H

#include "main.h"
#include "manager.h"
#include "input.h"
#include "scene2D.h"
#include "object.h"
#include "animal.h"

#define ATTACK_INTERVAL_TIME (1)
#define BASE_LIFE (1)
#define BOSS_SIZ (300)

class CBoss : public CAnimal {
public:
	CBoss(int nPriority = CManager::PRIORITY_BOSS);
	~CBoss();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBoss* Create(D3DXVECTOR3 pos, BOSS_TYPE Type, OBJTYPE ObjType);
	static HRESULT Load(void);
	static void UnLoad(void);

	void SetType(BOSS_TYPE Type);
	BOSS_TYPE GetEnemyType(void);
	static int GetNumAll(void);
	void AttackTypeSwitch(D3DXVECTOR3 pos);
	D3DXVECTOR3 MoveTypeSwitch(D3DXVECTOR3 pos, D3DXVECTOR3 Playerpos);
	static void SetJudgeFinsh(bool bJudge);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[BOSSTYPE_MAX];	// テクスチャへのポインタ

	BOSS_TYPE m_Type;
	static int m_nNumAll;
	static bool m_bJudgeFinsh;
	bool m_bUseJudge;
	int m_nFrame;
	int m_nAngle;
};

#endif