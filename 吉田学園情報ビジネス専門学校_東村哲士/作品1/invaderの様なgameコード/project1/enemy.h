#ifndef ENEMY_H
#define ENEMY_H

#include "main.h"
#include "manager.h"
#include "scene2D.h"
#include "animal.h"

#include <cmath>

#define ANGLE_DISCREPANCY (-24.7f)
#define ENEMY_SIZ (50)
#define ENEMY_VECTOR (5)

class CEnemy : public CAnimal {
public:
	CEnemy(int nPriority = CManager::PRIORITY_ENEMY);
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy* Create(D3DXVECTOR3 pos, ENEMY_TYPE EnemyType, OBJTYPE ObjType, int nStandard = FALSE_MOVEMENT);
	static HRESULT Load(void);
	static void UnLoad(void);

	static int GetNumAll(void);
	void AddNumAll(int nNum);
	void AttackTypeSwitch(D3DXVECTOR3 pos);
	D3DXVECTOR3 MoveTypeSwitch(D3DXVECTOR3 pos, D3DXVECTOR3 Playerpos);

	void SetType(ENEMY_TYPE Type);
	ENEMY_TYPE GetEnemyType(void);

	static int nNumAll;
	static  int m_nCreatEnemyNum;
	D3DXVECTOR3 m_fastpos;
	float m_fAngle;
	static bool m_bCreatEnemy;

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[ENEMYTYPE_MAX];	// テクスチャへのポインタ

	D3DXVECTOR3 m_move = D3DXVECTOR3(ENEMY_VECTOR, ENEMY_VECTOR, 0);
	int m_nPriority;

	int m_nStandardY;
	bool m_bScreen_Judgment;

	CSound* m_pSound;
	ENEMY_TYPE m_Type;
};

#endif