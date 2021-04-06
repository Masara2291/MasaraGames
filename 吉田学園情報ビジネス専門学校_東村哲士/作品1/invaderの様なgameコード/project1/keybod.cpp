#include "keybord.h"

CKeybord::CKeybord() {
	//�������̃f�[�^�����̐��l�ɏ���������
	//memset(�������������z��̕ϐ�, ����������l, �������������z��̕ϐ��̃o�C�g��)
	memset(m_akeyState, 0, sizeof(m_akeyState));
	memset(m_akeyStateTrigger, 0, sizeof(m_akeyStateTrigger));
	memset(m_akeyStateRelease, 0, sizeof(m_akeyStateRelease));
}

CKeybord::~CKeybord() {

}

//�L�[�{�[�h�̏�����
HRESULT CKeybord::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Directlnput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL))) {
		return E_FAIL;
	}
	//���̓f�o�C�X�̍쐬
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL))) {
		return E_FAIL;
	}
	//�f�[�^�E�t�H�[�}�b�g�ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	//�������[�h�ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		return E_FAIL;
	}
	//�L�[�{�[�h�ւ̃A�N�Z�X���l��
	m_pDevice->Acquire();

	return S_OK;
}

void CKeybord::Update(void)
{
	BYTE akeyState[NUM_KEY_MAX];//�L�[�̍ő吔
	int nCntkey;

	//�f�o�C�X����f�[�^�擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(akeyState), akeyState))) {
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++) {
			m_akeyStateTrigger[nCntkey] = (m_akeyState[nCntkey] ^ akeyState[nCntkey]) & akeyState[nCntkey];
			m_akeyStateRelease[nCntkey] = (m_akeyState[nCntkey] ^ akeyState[nCntkey]) &~akeyState[nCntkey];
			//�L�[�v���X����ۑ�
			m_akeyState[nCntkey] = akeyState[nCntkey];
		}
	}
	//�L�[�{�[�h�X�V����
	else {
		//�L�[�{�[�h�̃A�N�Z�X���擾
		m_pDevice->Acquire();
	}
}

bool CKeybord::GetkeyboardPress(int nkey)
{
	return (m_akeyState[nkey] & 0x80) ? true : false;
}

bool CKeybord::GetkeyboardTrigger(int nkey)
{
	return (m_akeyStateTrigger[nkey] & 0x80) ? true : false;
}

bool CKeybord::GetkeyboardRelease(int nkey)
{
	return (m_akeyStateRelease[nkey] & 0x80) ? true : false;
}

void CKeybord::Uninit(void)
{
	//���̓f�o�C�X�J��
	if (m_pDevice != NULL) {
		//�L�[�{�[�h�̃A�N�Z�X���J��
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//Directlnput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL) {
		m_pInput->Release();
		m_pInput = NULL;
	}
}