#ifndef _INPUT_H_
#define _INPUT_H_

#define NUM_KEY_MAX (256)//キーボードの最大数

#include "input.h"

class CKeybord : public CInput {
public:
	CKeybord();
	~CKeybord();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetkeyboardPress(int nkey);
	bool GetkeyboardTrigger(int nkey);
	bool GetkeyboardRelease(int nkey);

private:
	BYTE m_akeyState[NUM_KEY_MAX];//キーボードの入力情報ワーク
	BYTE m_akeyStateTrigger[NUM_KEY_MAX];//キーボードのトリガー情報
	BYTE m_akeyStateRelease[NUM_KEY_MAX];//キーボードのリリース情報
};

#endif