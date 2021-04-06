#define _WINSOCK_DEPRECATED_NO_WARNINGS

//=============================================================================
//
// �N���C�A���g���� [client.cpp]
// Author : �����ȉ�
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "tcp_client.h"

//=============================================================================
// tcp_client�̃R���X�g���N�^
//=============================================================================
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//=============================================================================
// tcp_client�̃f�X�g���N�^
//=============================================================================
CTcpClient::~CTcpClient()
{

}

//=============================================================================
// tcp_client�̏���������
//=============================================================================
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	// �ڑ��p�\�P�b�g�̐���
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		return false;
	}

	// �ڑ�����̐ݒ�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;                        // IPv4
	addr.sin_port = htons(nPortNum);                  // �|�[�g�ԍ�
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName); // ���ׂĂ̐ڑ����󂯂���

	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		return false;
	}
	return true;
}

//=============================================================================
// tcp_client�̃��N�G�X�g����
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
// tcp_client�̃��X�|���X����
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
		// �ؒf
		return 0;
	}
	return nRecvSize;
}

//=============================================================================
// tcp_client�̃N���G�C�g����
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
// tcp_client�̃����[�X����
//=============================================================================
void CTcpClient::Release(void)
{
	Close();
	delete this;
}

//=============================================================================
// tcp_client�̃N���[�Y����
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