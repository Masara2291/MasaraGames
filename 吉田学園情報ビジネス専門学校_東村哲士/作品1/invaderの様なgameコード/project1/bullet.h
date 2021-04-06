#ifndef BULLET_H
#define BULLET_H

#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

#define BULLET_TEXTURE_NUM (2)
#define GRAVITY_SIZ (300.0f)
#define BULLET_SIZ    (25.0f)
#define BULLET_VECTOR (8)
#define BULLET_LIFE   (40)

class CScore;

class CBullet : public CScene2d {
public:
	typedef enum {
		BULLET_TYPE_NORMAL = 0,
		BULLET_TYPE_TRACKING,
		BULLET_TYPE_INITIAL_TAILGATING,
		BULLET_TYPE_GRAVITY,
		BULLET_TYPE_UP_DOWN,
		BULLET_TYPE_INVOLUTE_CURVE,
		BULLET_TYPE_MAX
	}BULLET_TYPE;

	CBullet(int nPriority = CManager::PRIORITY_BULLET);
	~CBullet();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBullet* Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, OBJTYPE ObjType, D3DCOLOR col, BULLET_TYPE BulletType = BULLET_TYPE_NORMAL, int nLife = BULLET_LIFE, D3DXVECTOR3 siz = D3DXVECTOR3(BULLET_SIZ, BULLET_SIZ, 0));
	static HRESULT Load(void);
	static void UnLoad(void);

	BULLET_TYPE GetBulletType(void);
	BULLET_TYPE m_Type;
	int m_nLife;

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[BULLET_TEXTURE_NUM];		// テクスチャへのポインタ

	D3DCOLOR m_col;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_siz;

	float m_fAngle;
	int m_nFrame;
	int m_nPriority;
	CEnemy* m_pEnemy;
	CSound* m_pSound;
};

#endif