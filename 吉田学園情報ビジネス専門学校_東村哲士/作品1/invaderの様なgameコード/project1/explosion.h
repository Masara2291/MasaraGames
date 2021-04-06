#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "main.h"
#include "scene2D.h"
#include "manager.h"
#include "bullet.h"

#define EXPLOSION_SIZ    (35)
#define EFFECT_SIZ (100)

class CExplosion : public CScene2d {
public:
	CExplosion(int nPriority = CManager::PRIORITY_EXPLOSION);
	~CExplosion();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CExplosion* Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz = D3DXVECTOR3(EXPLOSION_SIZ, EXPLOSION_SIZ, 0));

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ

	CBullet* m_pBullet;
	CSound* m_pSound;
	int m_nCounterAnim = 0;//アニメーションカウンター
	int m_nPatternAnim = 0;//アニメーションNo.

	int m_nPriority;
};

#endif