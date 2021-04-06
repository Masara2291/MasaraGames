#ifndef _TITLE_H_
#define _TITLE_H_

#include "object.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"

#define TITLE_NUM (1)
#define TITLE_SIZE_X (1280.0f)
#define TITLE_SIZE_Y (720.0f)
#define KEY_SIZ_X (700.0f)
#define KEY_SIZ_Y (350.0f)

class CTitle {
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CTitle* Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TITLE_NUM];//テクスチャへのポインタ
	CScene2d* m_apScene2d[TITLE_NUM];

	CInput* m_pInput;

	int m_nPriority;
};

#endif