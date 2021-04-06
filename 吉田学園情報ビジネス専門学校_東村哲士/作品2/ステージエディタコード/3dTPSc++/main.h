//=============================================================================
//
// メイン処理 [main.h]
// Author : 後藤慎之助　//マサラタウンのサトシ
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <WinSock2.h>
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <thread>

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib, "d3d9.lib")    // 描画処理に必要
#pragma comment(lib, "d3dx9.lib")   // 拡張ライブラリ
#pragma comment(lib, "dxguid.lib")  // コンポーネント使用に必要
#pragma comment(lib,"dinput8.lib")  // 入力処理に必要
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ウインドウの位置
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)

// 画面の幅
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)

// 頂点とポリゴン
#define NUM_VERTEX	(4)
#define NUM_POLYGON	(2)

//頂点フォーマット		(頂点座標|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D   (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// デフォルト設定
#define DEFAULT_COLOR (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define DEFAULT_VECTOR (D3DXVECTOR3(0.0f,0.0f,0.0f))

// 重力
#define GRAVITY (1.1f)

// 点滅時間
#define BLINK_TIME 15

//円周率
#define M_PI 3.14159265358979

//*****************************************************************************
// 構造体定義
//*****************************************************************************

typedef struct
{
    D3DXVECTOR3 pos;
    float rhw;
    D3DCOLOR col;
    D3DXVECTOR2 tex;
}VERTEX_2D;

typedef struct
{
    D3DXVECTOR3 pos;
    D3DXVECTOR3 nor;
    D3DCOLOR    col;
    D3DXVECTOR2 tex;
}VERTEX_3D;

typedef struct
{
	int nNotch;     //ノッチ数
	int zDelta;     //回転量
	int nCount = 0; //回転数
}MOUSEWHEEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
#ifdef _DEBUG
int GetFPS(void);    // FPS取得
#endif

MOUSEWHEEL GetMouseWheel(void);

#endif