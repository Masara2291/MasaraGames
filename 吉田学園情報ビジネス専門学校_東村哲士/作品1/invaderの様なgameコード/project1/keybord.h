#ifndef _INPUT_H_
#define _INPUT_H_

#define NUM_KEY_MAX (256)//�L�[�{�[�h�̍ő吔

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
	BYTE m_akeyState[NUM_KEY_MAX];//�L�[�{�[�h�̓��͏�񃏁[�N
	BYTE m_akeyStateTrigger[NUM_KEY_MAX];//�L�[�{�[�h�̃g���K�[���
	BYTE m_akeyStateRelease[NUM_KEY_MAX];//�L�[�{�[�h�̃����[�X���
};

#endif