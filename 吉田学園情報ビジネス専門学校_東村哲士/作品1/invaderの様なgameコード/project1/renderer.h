#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CRenderere {
public:
	CRenderere();
	~CRenderere();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

private:
	void DrawFPS(void);

	LPDIRECT3D9				m_pD3D = NULL;			// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)

#ifdef _DEBUG
	LPD3DXFONT				m_pFont = NULL;			// フォントへのポインタ
#endif
};

#endif