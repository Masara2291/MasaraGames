#include "keybord.h"

CKeybord::CKeybord() {
	//メモリのデータを特定の数値に初期化する
	//memset(初期化したい配列の変数, 初期化する値, 初期化したい配列の変数のバイト数)
	memset(m_akeyState, 0, sizeof(m_akeyState));
	memset(m_akeyStateTrigger, 0, sizeof(m_akeyStateTrigger));
	memset(m_akeyStateRelease, 0, sizeof(m_akeyStateRelease));
}

CKeybord::~CKeybord() {

}

//キーボードの初期化
HRESULT CKeybord::Init(HINSTANCE hInstance, HWND hWnd)
{
	//Directlnputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL))) {
		return E_FAIL;
	}
	//入力デバイスの作成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL))) {
		return E_FAIL;
	}
	//データ・フォーマット設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard))) {
		return E_FAIL;
	}
	//強調モード設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))) {
		return E_FAIL;
	}
	//キーボードへのアクセス権獲得
	m_pDevice->Acquire();

	return S_OK;
}

void CKeybord::Update(void)
{
	BYTE akeyState[NUM_KEY_MAX];//キーの最大数
	int nCntkey;

	//デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(akeyState), akeyState))) {
		for (nCntkey = 0; nCntkey < NUM_KEY_MAX; nCntkey++) {
			m_akeyStateTrigger[nCntkey] = (m_akeyState[nCntkey] ^ akeyState[nCntkey]) & akeyState[nCntkey];
			m_akeyStateRelease[nCntkey] = (m_akeyState[nCntkey] ^ akeyState[nCntkey]) &~akeyState[nCntkey];
			//キープレス情報を保存
			m_akeyState[nCntkey] = akeyState[nCntkey];
		}
	}
	//キーボード更新処理
	else {
		//キーボードのアクセス権取得
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
	//入力デバイス開放
	if (m_pDevice != NULL) {
		//キーボードのアクセス権開放
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//Directlnputオブジェクトの開放
	if (m_pInput != NULL) {
		m_pInput->Release();
		m_pInput = NULL;
	}
}