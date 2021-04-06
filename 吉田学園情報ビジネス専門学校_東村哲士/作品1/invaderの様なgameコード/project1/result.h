#ifndef _RESULT_H_
#define _RESULT_H_

#include "object.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "scene2D.h"

#define RESULT_NUM (2)
#define RESULT_SIZE_X (1280.0f)
#define RESULT_SIZE_Y (720.0f)
#define KEY_SIZ_X (700.0f)
#define KEY_SIZ_Y (350.0f)

class CResult {
public:
	CResult();
	~CResult();

	HRESULT Init(int nNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResult* Create(int nNumber);
	static HRESULT Load(void);
	static void UnLoad(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[RESULT_NUM];//テクスチャへのポインタ
	CScene2d* m_pScene2d;
	CInput* m_pInput;

	int m_nPriority;
};

#endif