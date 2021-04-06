//=============================================================================
//
// �����L���O���� [ranking.h]
// Author : ���� �ȉ�
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "number.h"
#include "object.h"
#include "tcp_client.h"
#include "protocol_network.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_WIDTH				(650)		// �����L���O�̕�
#define RANKING_HEIGTH				(150)		// �����L���O�̍���
#define RANKINGNUMBER_SIZ_X			(60)
#define RANKINGNUMBER_SIZ_Y			(60)
#define RANKING_MAX_TEXTURE			(1)
#define BUFFER_NUM                  (256)
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRanking
{
public:
	CRanking();
	~CRanking();
	static HRESULT Load(void);
	static void UnLoad(void);
	static CRanking *Create();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void GetRanking(RankingData *pRankingData);
	static int SetRanking(int nScore);
	static 	RankingData		m_aRankingData[MAX_RANKING];

private:
	CScene2d					*m_apScene2D[RANKING_MAX_TEXTURE];
	CNumber						*m_apNumber[MAX_SCORE][MAX_RANKING];
	CInput						*m_pInput;
	static CScore				*m_pRanking[MAX_RANKING];					// �����L���O�̒l
	static int					m_nScore;									// �擾����X�R�A�̒l
	static char					m_aSendBuf[BUFFER_NUM];						// ���M����o�b�t�@�T�C�Y
	static char					m_aRecvBuf[BUFFER_NUM];						// ��M����o�b�t�@�T�C�Y
	static LPDIRECT3DTEXTURE9	m_apTexture[RANKING_MAX_TEXTURE];					// �e�N�X�`��
};
#endif