#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "animal.h"

#define DEATH_STAGE (2)

#define ATTACK_INTERVAL (11)
#define PLAYER_DRAW_SIZ_X (50.0f)
#define PLAYER_DRAW_SIZ_Y (50.0f)
#define PLAYER_JUDGE_SIZ_X (25.0f)
#define PLAYER_JUDGE_SIZ_Y (25.0f)

class CScore;

class CPlayer : public CAnimal {
public:
	CPlayer(int nPriority = CManager::PRIORITY_PLAYER);
	~CPlayer();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	static CPlayer* Create(D3DXVECTOR3 pos, OBJTYPE ObjType);
	static int GetDeathStage(int nNum);
	static void SetDeathStage(int nNum1, int nNum2);
	static void SetMove(D3DXVECTOR3 move);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ

	float fAngle = 0;
	D3DXVECTOR3 PolygonMove = D3DXVECTOR3(5, 5, 0);
	float fRd;
	float fAngleX0, fAngleY0;
	float fAngleX1, fAngleY1;
	float fAngleX2, fAngleY2;
	float fAngleX3, fAngleY3;
	static int m_DeathStage[DEATH_STAGE];
	static D3DXVECTOR3 m_move;

	CInput* m_pInput;
	CSound* m_pSound;
	CScore* m_pScore;

	int m_nPriority;
	int m_nCount;
};

#endif