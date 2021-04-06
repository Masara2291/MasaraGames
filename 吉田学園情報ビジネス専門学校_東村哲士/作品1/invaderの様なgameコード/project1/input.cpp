#include "input.h"

LPDIRECTINPUT8 CInput::m_pInput = NULL;

CInput::CInput() {
	m_pDevice = NULL;
}

CInput::~CInput() {

}