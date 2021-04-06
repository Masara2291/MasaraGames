#ifndef BG_H
#define BG_H

#define BG_NUM (3)

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "object.h"

class CBg : public CObject {
public:
	CBg(int nPriority = CManager::PRIORITY_BG);
	~CBg();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBg* Create();
	static HRESULT Load(void);
	static void UnLoad(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[BG_NUM];
	CScene2d* m_apScene2d[BG_NUM];

	int m_nCounterAnimBg = 0;//アニメーションカウンター
	int m_nPatternAnimBg = 0;//アニメーションNo.
	int m_MoveVector[BG_NUM];
	int m_nType;

	int m_nPriority;
};

#endif // !BG_H