//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : 佐藤 諒佳
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "number.h"
#include "object.h"
#include "tcp_client.h"
#include "protocol_network.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RANKING_WIDTH				(650)		// ランキングの幅
#define RANKING_HEIGTH				(150)		// ランキングの高さ
#define RANKINGNUMBER_SIZ_X			(60)
#define RANKINGNUMBER_SIZ_Y			(60)
#define RANKING_MAX_TEXTURE			(1)
#define BUFFER_NUM                  (256)
//*****************************************************************************
// クラス定義
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
	static CScore				*m_pRanking[MAX_RANKING];					// ランキングの値
	static int					m_nScore;									// 取得するスコアの値
	static char					m_aSendBuf[BUFFER_NUM];						// 送信するバッファサイズ
	static char					m_aRecvBuf[BUFFER_NUM];						// 受信するバッファサイズ
	static LPDIRECT3DTEXTURE9	m_apTexture[RANKING_MAX_TEXTURE];					// テクスチャ
};
#endif