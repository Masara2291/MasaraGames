#define _WINSOCK_DEPRECATED_NO_WARNINGS

//=============================================================================
//
// クライアント処理 [client.cpp]
// Author : 佐藤諒佳
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "tcp_client.h"

//=============================================================================
// tcp_clientのコンストラクタ
//=============================================================================
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//=============================================================================
// tcp_clientのデストラクタ
//=============================================================================
CTcpClient::~CTcpClient()
{

}

//=============================================================================
// tcp_clientの初期化処理
//=============================================================================
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	// 接続用ソケットの生成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		return false;
	}

	// 接続先情報の設定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;                        // IPv4
	addr.sin_port = htons(nPortNum);                  // ポート番号
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName); // すべての接続を受けつける

	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		return false;
	}
	return true;
}

//=============================================================================
// tcp_clientのリクエスト処理
//=============================================================================
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		return 0;
	}
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);
	return nSendSize;
}

//=============================================================================
// tcp_clientのレスポンス処理
//=============================================================================
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	if (m_sock < 0)
	{
		return 0;
	}
	int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);
	if (nRecvSize <= 0)
	{
		// 切断
		return 0;
	}
	return nRecvSize;
}

//=============================================================================
// tcp_clientのクリエイト処理
//=============================================================================
CTcpClient * CTcpClient::Create(const char *pHostName, int nPortNum)
{
	CTcpClient *pTcpClient = new CTcpClient;
	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}
	return pTcpClient;
}

//=============================================================================
// tcp_clientのリリース処理
//=============================================================================
void CTcpClient::Release(void)
{
	Close();
	delete this;
}

//=============================================================================
// tcp_clientのクローズ処理
//=============================================================================
void CTcpClient::Close()
{
	if (m_sock < 0)
	{
		return;
	}
	closesocket(m_sock);
	m_sock = -1;
}