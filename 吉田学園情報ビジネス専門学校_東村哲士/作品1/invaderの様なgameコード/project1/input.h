#ifndef INPUT_H
#define INPUT_H

#include "main.h"

class CInput {
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice = NULL;//���̓f�o�C�X�ւ̃|�C���^
	static LPDIRECTINPUT8 m_pInput;//DirectIPut�I�u�W�F�N�g

private:
};

#endif