#ifndef CREATSTAGE_H
#define CREATSTAGE_H

#include "main.h"
#include "manager.h"
#include "input.h"
#include "scene2D.h"
#include "object.h"
#include "enemy.h"
#include "animal.h"

#include <stdio.h>
#include <string.h>

#define TEXT_NUM "stage/2/2-1.txt"
#define MAX_TEXTURE (2)
#define CURSOR_SIZ (40)
#define MASS_SIZ (10)

class CPlayer;
class CMessage;

class CCreateStage {
public:
	typedef enum
	{
		MAP_0 = 0,
		MAP_1,
		MAP_2,
		MAP_3,
		MAP_MAX
	} MAP;

	typedef enum
	{
		STAGE_0 = 0,
		STAGE_1,
		STAGE_2,
		STAGE_3,
		STAGE_MAX
	} STAGE;

	typedef struct
	{
		D3DXVECTOR3 pos;	        // 敵の座標
		CAnimal::ENEMY_TYPE Type;	// 敵の種類
		int nNum;                   // 敵の総数
	} STAGEDATE;

	typedef struct
	{
		D3DXVECTOR3 pos;
		CAnimal::ENEMY_TYPE nType;
	} ENEMYDATE;

	CCreateStage();
	~CCreateStage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static HRESULT Load(void);
	static void UnLoad(void);
	static HRESULT LoadStage(void);
	static bool GetBoolean(void);

	HRESULT SaveStage(void);
	HRESULT CreateStage(void);

	static void SetEnemy(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];//テクスチャへのポインタ
	CScene2d* m_pScene2d01[MAX_TEXTURE];
	CScene2d* m_pScene2d02;
	D3DXVECTOR3 m_pos;

	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;
	CInput* m_pInput;
	CMessage* m_pMessage;

	ENEMYDATE m_EnemyDate;
	
	static bool m_bEnemyBox;
	bool m_bJudge = false;
};

#endif