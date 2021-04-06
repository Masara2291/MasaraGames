//=============================================================================
//
// �i���o�[���� [number.h]
// Author : ���� �ȉ�
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "score.h"
#include "scene2d.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUMBER_WIDTH		(25)		// �����̕�
#define NUMBER_HEIGHT		(25)		// �����̍���

//*****************************************************************************
// �N���X��`
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