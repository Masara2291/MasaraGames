//=============================================================================
//
// クライアント処理 [client.h]
// Author : 佐藤諒佳
//
//=============================================================================
#ifndef _CLIENT_H_
#define _CLIENT_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	static CTcpClient *Create(const char *pHostName, int nPortNum);
	bool Init(const char *pHostName, int nPortNum);
	int Send(char *pSendData, int nSendDataSize);
	int Recv(char *pRecvBuf, int nRecvBufSize);
	void Release(void);
	void Close(void);
private:
	SOCKET m_sock;
};
#endif