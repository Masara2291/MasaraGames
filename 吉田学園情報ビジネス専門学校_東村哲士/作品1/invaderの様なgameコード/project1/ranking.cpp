//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : ���� �ȉ�
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "ranking.h"
#include "keybord.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
CScore				*CRanking::m_pRanking[MAX_RANKING] = {};
char				CRanking::m_aSendBuf[BUFFER_NUM] = {};
char				CRanking::m_aRecvBuf[BUFFER_NUM] = {};
int					CRanking::m_nScore = 0;
RankingData			CRanking::m_aRankingData[MAX_RANKING] = {};
LPDIRECT3DTEXTURE9	CRanking::m_apTexture[RANKING_MAX_TEXTURE] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{
	m_nScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init()
{
	// �X�R�A�̏����擾
	m_nScore = CScore::GetScore();
	// �����L���O����ݒ�
	SetRanking(m_nScore);
	// �����L���O�����擾
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
// �I������
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
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	// �L�[�{�[�h�̏�Ԃ��擾
	m_pInput = CManager::GetKeybord();

	for (int nRank = 0; nRank < MAX_RANKING; nRank++)
	{
		for (int nCount = 0; nCount < MAX_SCORE; nCount++)
		{
			m_apNumber[nCount][nRank]->Update();
		}
	}

	if ((((CKeybord*)m_pInput)->GetkeyboardTrigger(DIK_RETURN))) {
		CManager::SetMode(CManager::MODE_TITLE);//���U���g��ʂɈړ�
		return;
	}
}

//=============================================================================
// �`�揈��
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
// �����L���O�����擾
//=============================================================================
void CRanking::GetRanking(RankingData *pRankingData)
{
	WSADATA wsaData;

	// winsock�̏�����
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		// ���炩�̌����ŏ������Ɏ��s����
		printf("WSAStartup falied\n");
	}

	// �N���G�C�g
	//CTcpClient *pTcpClient = CTcpClient::Create("127.0.0.1", 12345);
	CTcpClient *pTcpClient = CTcpClient::Create("20.48.3.137", 12345);

	m_aRankingData[MAX_RANKING] = pRankingData[MAX_RANKING];

	// �����L���O�ꗗ�擾���N�G�X�g���M
	m_aSendBuf[0] = COMMAND_TYPE_GET_RANKING;

	// send�����s
	pTcpClient->Send(m_aSendBuf, strlen(m_aSendBuf));

	// �����L���O�ꗗ�擾���X�|���X��M
	memset(m_aRecvBuf, 0, sizeof(m_aRecvBuf));

	// recv�����s
	pTcpClient->Recv(m_aRecvBuf, sizeof(m_aRecvBuf));

	// pRankingData[nCntRank]��aRecvBuf�̃f�[�^��ݒ�
	int nIndex = 0;
	for (int nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		m_nScore = *(int*)&m_aRecvBuf[nIndex];
		m_nScore = ntohl(m_nScore);
		m_aRankingData[nCntRank].nClearScore = m_nScore;
		nIndex += sizeof(int);
	}
	// �����[�X����
	pTcpClient->Release();
	pTcpClient = NULL;
}

//=============================================================================
// �����L���O�̏���ݒ�
//=============================================================================
int CRanking::SetRanking(int nScore)
{
	WSADATA wsaData;
	// winsock�̏�����
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	if (nErr != 0)
	{
		// ���炩�̌����ŏ������Ɏ��s������
		printf("WSAStartup failed\n");
	}

	// TCP����M�N���X�̐���
	//CTcpClient *pTcpClient = CTcpClient::Create("127.0.0.1", 12345);
	CTcpClient *pTcpClient = CTcpClient::Create("20.48.3.137", 12345);

	if (pTcpClient == NULL)
	{
		return 0;
	}

	// �����L���O�ݒ胊�N�G�X�g�̑��M
	// 1�o�C�g��:COMMAND_TYPE_SET_RANKING��ݒ�
	m_aSendBuf[0] = COMMAND_TYPE_SET_RANKING;					// 1�o�C�g���M
	// 2�`5�o�C�g��:�X�R�A��ݒ�(htonl�֐����g�p)
	nScore = htonl(nScore);
	memcpy(&m_aSendBuf[1], &nScore, sizeof(int));				// 4�o�C�g���M

	// 5�o�C�g���M
	pTcpClient->Send(m_aSendBuf, 5);

	// �����L���O�ݒ�̃��X�|���X����M(Recv���g�p)
	// �[���N���A
	memset(m_aRecvBuf, 0, sizeof(m_aRecvBuf));
	pTcpClient->Recv(m_aRecvBuf, sizeof(m_aRecvBuf));
	// �ؒf����
	pTcpClient->Release();
	// return�ŏ��ʂ�Ԃ�
	return (int)m_aRecvBuf[0];
}

//=============================================================================
// �N���G�C�g�֐�
//=============================================================================
CRanking * CRanking::Create()
{
	CRanking *pRanking;
	pRanking = new CRanking;
	pRanking->Init();
	return pRanking;
}

//=============================================================================
// �e�N�X�`�������[�h
//=============================================================================
HRESULT CRanking::Load(void)
{
	CRenderere* pRenderere = CManager::GetRenderere();
	if (m_apTexture[0] == NULL) {
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[0]);
	}
	if (m_apTexture[1] == NULL) {
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[1]);
	}
	if (m_apTexture[2] == NULL) {
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pRenderere->GetDevice(), "texture/result004.png", &m_apTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����A�����[�h
//=============================================================================
void CRanking::UnLoad(void)
{
	for (int nTex = 0; nTex < RANKING_MAX_TEXTURE; nTex++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nTex] != NULL)
		{
			m_apTexture[nTex]->Release();
			m_apTexture[nTex] = NULL;
		}
	}
}