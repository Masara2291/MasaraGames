#ifndef MANAGER_H
#define MANAGER_H

#include "main.h"
#include "input.h"
#include "renderer.h"
#include "scene2d.h"
#include "object.h"

#define PLAYER_ALL_NUM (2)

#define RAND_POS
#undef RAND_POS

#define ENUMERATE
#undef ENUMERATE

class CMouse;
class CKeybord;
class CPlayer;
class CBullet;
class CExplosion;
class CBg;
class CEnemy;
class CTitle;
class CResult;
class CCreateStage;
class CSound;
class CScore;
class CBg;

class CManager {
public:
	typedef enum {
		MODE_TITLE,
		MODE_GAME,
		MODE_TESTGAME,
		MODE_CREATE,
		MODE_RESULT,
		MODE_RANKING,
		MODE_TESTRESULT,
		MODE_MAX
	}MODE;

	typedef enum {
		PRIORITY_BG,
		PRIORITY_ENEMY,
		PRIORITY_ENEMY_CREAT,
		PRIORITY_PLAYER,
		PRIORITY_BOSS,
		PRIORITY_BULLET,
		PRIORITY_EXPLOSION,
		PRIORITY_SCORE,
		PRIORITY_CURSOR,
		PRIORITY_MESSAGE,
		PRIORITY_MAX
	}PRIORITY;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static void SetMode(MODE mode);

	static CRenderere* GetRenderere(void);
	static CKeybord* GetKeybord(void);
	static CMouse* GetMouse(void);
	static CPlayer* GetPlayer(void);
	static CBullet* GetBullet(void);
	static CEnemy* GetEnemy(void);
	static CSound* GetSound(void);
	static MODE GetMode(void);
	static MODE GetOldMode(void);
	static CScore* GetScore(void);
	void Load(void);
	void UnLoad(void);

	static CTitle* m_pTitle;
	static CResult* m_pResult;
	static MODE m_oldmode;
	static MODE m_mode;
	static CSound* m_pSound;

private:
	static CRenderere* m_pRenderer;
	static CMouse* m_pMouse;
	static CKeybord* m_pKeybord;
	static CPlayer* m_pPlayer;
	static CBullet* m_pBullet;
	static CEnemy* m_pEnemy;
	static CCreateStage* m_pCreatStage;
	static CScore* m_pScore;
	static CBg* m_pBg;
};

#endif