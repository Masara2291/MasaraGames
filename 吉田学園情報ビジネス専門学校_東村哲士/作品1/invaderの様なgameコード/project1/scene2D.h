#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "renderer.h"
#include "object.h"
#include "keybord.h"

#define FALSE_MOVEMENT (-1)

#define CHANGE_COL
#undef CHANGE_COL

#define MOVE_JUDGE
#undef MOVE_JUDGE

#define _POLYGON_POS_X (SCREEN_WIDTH / 2)
#define _POLYGON_POS_Y (SCREEN_HEIGHT / 2)

#define MOTIVE_POWER (7) //移動量

class CScene2d : public CObject {
public:
	CScene2d(int nPriority = 3);
	virtual ~CScene2d();

	virtual HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2d* Create(int nPriority = 0);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetCOL(D3DCOLOR col);
	D3DCOLOR GetCOL(void);
	void SetSIZ(D3DXVECTOR3 siz);
	D3DXVECTOR3 GetSIZ(void);
	void SetPOS(D3DXVECTOR3 pos);
	void SetTex(float texpos_x, float texpos_y, int texnum);
	D3DXVECTOR3 GetPos(void);

	bool BooleanToggle(bool bBoolVariable);
	float GetAngle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	int GetRand(int min_val, int max_val);

protected:
	
private:
	LPDIRECT3DTEXTURE9      m_pTexture; // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;	    // ポリゴンの位置
	D3DCOLOR m_col;
	D3DXVECTOR3 m_siz;
	float m_texpos_x, m_texpos_y;
	int m_texnum;
	float m_fTmp;
};

#endif