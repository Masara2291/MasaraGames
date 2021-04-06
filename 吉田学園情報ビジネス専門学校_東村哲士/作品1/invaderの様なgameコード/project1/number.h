//=============================================================================
//
// ナンバー処理 [number.h]
// Author : 佐藤 諒佳
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "score.h"
#include "scene2d.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUMBER_WIDTH		(25)		// 数字の幅
#define NUMBER_HEIGHT		(25)		// 数字の高さ

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CNumber
{
public:
	CNumber();
	~CNumber();
	static HRESULT Load(void);
	static void UnLoad(void);
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 siz);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(int nNumber);
private:
	static LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;
	D3DXVECTOR3					m_pos;
	int							m_nNumber;

	CScene2d* m_pScene2d;
};
#endif