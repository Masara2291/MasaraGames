#include "InputMouse.h"

CMouse::MSTATE CMouse::m_MState = {};

HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	m_pDevice->SetDataFormat(&c_dfDIMouse2); //ﾏｳｽ用のﾃﾞｰﾀ・ﾌｫｰﾏｯﾄ設定
	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 絶対値モードで設定（絶対値はDIPROPAXISMODE_ABS）（相対値はDIPROPAXISMODE_REL）
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	SetRect(&m_MState.moveRect, 0, 0, 1280, 720);	// マウスカーソルの動く範囲
	m_MState.x = m_MState.moveRect.left;	// マウスカーソルのＸ座標を初期化
	m_MState.y = m_MState.moveRect.top;	// マウスカーソルのＹ座標を初期化
	m_MState.moveAdd = 1;	// マウスカーソルの移動量を設定

	m_pDevice->Acquire();
	return S_OK;
}

/******************************************************************************
* 関数名 : UninitMouse
*
* 引数 : void
* 戻り値 : なし
* 説明 : 終了処理
*******************************************************************************/
void CMouse::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice = NULL;
	}

	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}


/******************************************************************************
* 関数名 : UpdateMouse
*
* 引数 : void
* 戻り値 : なし
* 説明 : 更新処理
*******************************************************************************/
void CMouse::Update(void)
{
	m_dIMouseState.rgbButtons[0] = 0;

	m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_dIMouseState);

	m_pDevice->Acquire();

	m_MState.x += (m_dIMouseState.lX * m_MState.moveAdd);
	m_MState.y += (m_dIMouseState.lY * m_MState.moveAdd);
	if (m_MState.x < m_MState.moveRect.left) m_MState.x = m_MState.moveRect.left;
	if (m_MState.x > m_MState.moveRect.right - m_MState.imgWidth) m_MState.x = m_MState.moveRect.right - m_MState.imgWidth;
	if (m_MState.y < m_MState.moveRect.top) m_MState.y = m_MState.moveRect.top;
	if (m_MState.y > m_MState.moveRect.bottom - m_MState.imgHeight) m_MState.y = m_MState.moveRect.bottom - m_MState.imgHeight;
	(m_dIMouseState.rgbButtons[0] & 0x80) ? m_MState.lButton = true : m_MState.lButton = false;
	(m_dIMouseState.rgbButtons[1] & 0x80) ? m_MState.rButton = true : m_MState.rButton = false;
	(m_dIMouseState.rgbButtons[2] & 0x80) ? m_MState.cButton = true : m_MState.cButton = false;
}

bool CMouse::GetMouseTriggerLeft(void)
{
	/* 前に取得した時のマウスの状態 */
	static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];
	/* 今のマウスの状態 */
	bool current = m_MState.lButton;
	/* 前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する */
	bool ret = current && !prevState[0];
	/* 今の状態を保存する */
	prevState[0] = current;
	/* 判定結果を返す */
	return ret;
}

bool CMouse::GetMouseTriggerRight(void)
{
	/* 前に取得した時のマウスの状態 */
	static bool prevState[sizeof(m_dIMouseState.rgbButtons) / sizeof(m_dIMouseState.rgbButtons[0])];
	/* 今のマウスの状態 */
	bool current = m_MState.rButton;
	/* 前の状態がfalseで、今の状態がtrueならば、クリックした瞬間と判定する */
	bool ret = current && !prevState[0];
	/* 今の状態を保存する */
	prevState[0] = current;
	/* 判定結果を返す */
	return ret;
}

CMouse::MSTATE *CMouse::GetMouse(void)
{
	return &m_MState;
}