#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "main.h"
#include "scene2D.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"

#define FALSE_USE (-1)
#define MESSAGE_SIZ_X (SCREEN_WIDTH / 4)
#define MESSAGE_SIZ_Y (SCREEN_HEIGHT / 4)
#define COL_ADD_NUM (15)
#define SIZ_ADD_NUM (3)
#define SIZ_SUBTRACTION_NUM (30)

class CMessage : public CScene2d {
public:

	typedef enum {
		MESSAGE_BLOCK,
		MESSAGE_SUCCESS_SAVE_STAGE,
		MESSAGE_MISS_SAVE_STAGE,
		MESSAGE_NOT_PUT_ENEMY,
		MESSAGE_MISS_READ_TXT,
		MESSAGE_WARNING,
		ERROR_NUM_MAX
	}MESSAGE_TYPE;

	CMessage(int nPriority = CManager::PRIORITY_MESSAGE);
	~CMessage();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CMessage* Create(MESSAGE_TYPE Message, D3DCOLOR col = D3DCOLOR_RGBA(255, 255, 255, 255), D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0), D3DXVECTOR3 siz = D3DXVECTOR3(MESSAGE_SIZ_X, MESSAGE_SIZ_Y, 0));
	static bool GetUseJudge(void);

	static bool m_bUseJudge;

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[ERROR_NUM_MAX];		// テクスチャへのポインタ

	CInput* m_pInput;
	D3DXVECTOR3 m_Argumentsiz;
	D3DXCOLOR m_Argumentcol;
	int m_ArgumentNum;

	int m_nPriority;
	int m_nJudge = 0;
};

#endif