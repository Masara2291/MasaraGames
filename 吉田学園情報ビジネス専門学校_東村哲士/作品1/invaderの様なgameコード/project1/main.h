#define DIRECTINPUT_VERSION (0x0800)

//=============================================================================
//
// ���C������ [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <WinSock2.h>// WinSocket���g�p���邽�߂�Include
#include <WS2tcpip.h>// WinSocket���g�p���邽�߂�Include
#include <windows.h>
#include <time.h>
#include <dinput.h>
#include <d3d9.h>       // DirectX8�w�b�_�t�@�C��
#include <d3dx9.h>      // D3DX�w�b�_�t�@�C��
#include <xaudio2.h>
#include <stdio.h>

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")//�V�X�e�������擾
#pragma comment(lib, "dinput8.lib")

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define FULSCREEN (true)
#define WINDOWSCREEN (false)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//���_��
	float rhw;//���W�ϊ��p�W��(1.0f�Œ�)
	D3DCOLOR col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
} VERTEX_2D;

class CRenderere;
class CScene;

//�֐�
int GetFPS(void);

#endif