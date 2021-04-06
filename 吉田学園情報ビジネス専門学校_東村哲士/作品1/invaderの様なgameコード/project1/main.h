#define DIRECTINPUT_VERSION (0x0800)

//=============================================================================
//
// メイン処理 [main.h]
// Author : 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <WinSock2.h>// WinSocketを使用するためのInclude
#include <WS2tcpip.h>// WinSocketを使用するためのInclude
#include <windows.h>
#include <time.h>
#include <dinput.h>
#include <d3d9.h>       // DirectX8ヘッダファイル
#include <d3dx9.h>      // D3DXヘッダファイル
#include <xaudio2.h>
#include <stdio.h>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")//システム時刻取得
#pragma comment(lib, "dinput8.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1280)
#define SCREEN_HEIGHT (720)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define FULSCREEN (true)
#define WINDOWSCREEN (false)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//頂点数
	float rhw;//座標変換用係数(1.0f固定)
	D3DCOLOR col;//頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
} VERTEX_2D;

class CRenderere;
class CScene;

//関数
int GetFPS(void);

#endif