//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : 佐藤 諒佳
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "ranking.h"
#include "keybord.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CScore				*CRanking::m_pRanking[MAX_RANKING] = {};
char				CRanking::m_aSendBuf[BUFFER_NUM] = {};
char				CRanking::m_aRecvBuf[BUFFER_NUM] = {};
int					CRanking::m_nScore = 0;
RankingData			CRanking::m_aRankingData[MAX_RANKING] = {};
LPDIRECT3DTEXTURE9	CRanking::m_apTexture[RANKING_MAX_TEXTURE] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_nScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init()
{
	// スコアの情報を取得
	m_nScore = CScore::GetScore();
	// ランキング情報を設定
	SetRanking(m_nScore);
	// ランキング情報を取得
	GetRanking(m_aRankingData);

	for (int nRank = 0; nRank < MAX_RANKING; nRank++)
	{
		for (int nCount = 0; nCount < MAX_SCORE; nCount++)
		{
			m_apNumber[nCount][nRank] = CNumber::Create(D3DXVECTOR3(RANKING_WIDTH - nCount * 60.0f, RANKING_HEIGTH + nRank * 120.0f, 0.0f), D3DXVECTOR3(RANKINGNUMBER_SIZ_X, RANKINGNUMBER_SIZ_Y, 0));
			m_apNumber[nCount][nRank]->SetNumber(m_aRankingData[nRank].nClearScore / ((int)pow(10, nCount % 10)));
		}
	}

	for (int nTex = 0; nTex < RANKING_MAX_TEXTURE; nTex++)
	{
		m_apScene2D[nTex] = CScene2d::Create();
		m_apScene2D[nTex]->SetPOS(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0));
		m_apScene2D[nTex]->SetSIZ(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0));
		m_apScene2D[nTex]->SetTexture(m_apTexture[nTex]);
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nRank = 0; nRank < MAX_RANKING; nRank++)
	{
		for (int nCount = 0; nCount < MAX_SCORE; nCount++)
		{
			if (m_apNumber[nCount][nRank] != NULL)
			{
				m_apNumber[nCount][nRank]->Uninit();

				delete m_apNumber[nCount][nRank];
				m_apNumber[nCount][nRank] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	// キーボードの状態を取得
	m_pInput = CManager::GetKeybord();

	for (int nRank = 0; nRank < MAX_RANKING; nRank++)
	{
		for (int nCount = 0; nCount < MAX_SCORE; nCount++)
		{
			m_apNumber[nCount][nRank]->Update();
		}
	}

	if ((((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_RETURN))) {
		CManager::SetMode(CManager::MODE_TITLE);//リザルト画面に移動
		return;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{
	for (int nRank = 0; nRank < MAX_RANKING; nRank++)
	{
		for (int nCount = 0; nCount < MAX_SCORE; nCount++)
		{
			m_apNumber[nCount][nRank]->Draw();
		}
	}
}

//=============================================================================
// ランキング情報を取得
//=============================================================================
void CRanking::GetRanking(RankingData *pRankingData)
{
	WSADATA wsaData;

	// winsockの初期化
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		// 何らかの原因で初期化に失敗した
		printf("WSAStartup falied\n");
	}

	// クリエイト
	//CTcpClient *pTcpClient = CTcpClient::Create("127.0.0.1", 12345);
	CTcpClient *pTcpClient = CTcpClient::Create("20.48.3.137", 12345);

	m_aRankingData[MAX_RANKING] = pRankingData[MAX_RANKING];

	// ランキング一覧取得リクエスト送信
	m_aSendBuf[0] = COMMAND_TYPE_GET_RANKING;

	// sendを実行
	pTcpClient->Send(m_aSendBuf, strlen(m_aSendBuf));

	// ランキング一覧取得レスポンス受信
	memset(m_aRecvBuf, 0, sizeof(m_aRecvBuf));

	// recvを実行
	pTcpClient->Recv(m_aRecvBuf, sizeof(m_aRecvBuf));

	// pRankingData[nCntRank]にaRecvBufのデータを設定
	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_nScore = *(int*)&m_aRecvBuf[nIndex];
		m_nScore = ntohl(m_nScore);
		m_aRankingData[nCntRank].nClearScore = m_nScore;
		nIndex += sizeof(int);
	}
	// リリース処理
	pTcpClient->Release();
	pTcpClient = NULL;
}

//=============================================================================
// ランキングの情報を設定
//=============================================================================
int CRanking::SetRanking(int nScore)
{
	WSADATA wsaData;
	// winsockの初期化
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		// 何らかの原因で初期化に失敗したら
		printf("WSAStartup failed\n");
	}

	// TCP送受信クラスの生成
	//CTcpClient *pTcpClient = CTcpClient::Create("127.0.0.1", 12345);
	CTcpClient *pTcpClient = CTcpClient::Create("20.48.3.137", 12345);

	if (pTcpClient == NULL)
	{
		return 0;
	}

	// ランキング設定リクエストの送信
	// 1バイト目:COMMAND_TYPE_SET_RANKINGを設定
	m_aSendBuf[0] = COMMAND_TYPE_SET_RANKING;					// 1バイト送信
	// 2～5バイト目:スコアを設定(htonl関数を使用)
	nScore = htonl(nScore);
	memcpy(&m_aSendBuf[1], &nScore, sizeof(int));				// 4バイト送信

	// 5バイト送信
	pTcpClient->Send(m_aSendBuf, 5);

	// ランキング設定のレスポンスを受信(Recvを使用)
	// ゼロクリア
	memset(m_aRecvBuf, 0, sizeof(m_aRecvBuf));
	pTcpClient->Recv(m_aRecvBuf, sizeof(m_aRecvBuf));
	// 切断処理
	pTcpClient->Release();
	// returnで順位を返す
	return (int)m_aRecvBuf[0];
}

//=============================================================================
// クリエイト関数
//=============================================================================
CRanking * CRanking::Create()
{
	CRanking *pRanking;
	pRanking = new CRanking;
	pRanking->Init();
	return pRanking;
}

//=============================================================================
// テクスチャをロード
//=============================================================================
HRESULT CRanking::Load(void)
{
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_apTexture[0] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[0]);
	}
	if (m_apTexture[1] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[1]);
	}
	if (m_apTexture[2] == NULL) {
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// テクスチャをアンロード
//=============================================================================
void CRanking::UnLoad(void)
{
	for (int nTex = 0; nTex < RANKING_MAX_TEXTURE; nTex++)
	{
		// テクスチャの破棄
		if (m_apTexture[nTex] != NULL)
		{
			m_apTexture[nTex]->Release();
			m_apTexture[nTex] = NULL;
		}
	}
}