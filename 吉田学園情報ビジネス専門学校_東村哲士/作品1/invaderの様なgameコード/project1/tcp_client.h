//=============================================================================
//
// �N���C�A���g���� [client.h]
// Author : �����ȉ�
//
//=============================================================================
#ifndef _CLIENT_H_
#define _CLIENT_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
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