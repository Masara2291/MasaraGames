//====================================================================
//
// 描画処理 (renderer.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// レンダリングクラス
class CRenderer
{
public:
    CRenderer();
    ~CRenderer();
    HRESULT Init(HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);    // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    void Draw(void);      // オブジェクトのインスタンスがないと、関数が呼べないため持ってくる
    LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
    LPDIRECT3D9				m_pD3D;			// Direct3Dオブジェクト
    LPDIRECT3DDEVICE9		m_pD3DDevice;	// Deviceオブジェクト(描画に必要)
#ifdef _DEBUG
    void DrawFPS(void);
#endif
	void DrawPosRAndSelectType(void);
	void DrawBlockPos(void);
	void DrawEnemyPos(void);
	void DrawCursorPos(void);
	void DrawFixedSshaft(void);
	void DrawSeveMapJudge(void);
	void DrawGreaseJudge(void);
	void DrawCopyDate(void);
	void DrawCreateObjectType(void);
	void DrawPauseJudge(void);
	void DrawObjectSaveDate(void);

	LPD3DXFONT m_pFont;        // フォントへのポインタ
	LPD3DXFONT m_pPauseFont;
	int m_nSaveMapJudgeTime;
	int m_nBlockSaveDateJudgeTime;
};

#endif