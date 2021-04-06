//===============================================
//
// 様々な処理をまとめたファイル [library.h]
// Author : 後藤慎之助
//
//===============================================
#ifndef _LIBRARY_H_
#define _LIBRARY_H_

//========================
// インクルードファイル
//========================
#include "main.h"

//========================
// マクロ定義
//========================
#define HIT_TOP D3DXVECTOR3(D3DXToRadian(90.0f), 0.0f, 0.0f)
#define HIT_LEFT D3DXVECTOR3(0.0f, D3DXToRadian(90.0f), 0.0f)
#define HIT_BACK D3DXVECTOR3(0.0f, D3DXToRadian(180.0f)
#define HIT_RIGHT D3DXVECTOR3(0.0f, D3DXToRadian(270.0f), 0.0f)
#define HIT_FRONT D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(0.0001f))

//========================
// 列挙型定義
//========================
// ブロックに当たった時の面
typedef enum
{
    HIT_SURFACE_NONE = 0,     // 当たっていない
    HIT_SURFACE_RIGHT,        // 右
    HIT_SURFACE_LEFT,         // 左
    HIT_SURFACE_FRONT,        // 手前
    HIT_SURFACE_BACK,         // 奥
    HIT_SURFACE_TOP,          // 上面
    HIT_SURFACE_BOTTOM,       // 底
}HIT_SURFACE;

//========================
// 構造体定義
//========================
// ブロックに当たった時の情報
typedef struct
{
    HIT_SURFACE hitSurface;     // 当たった面
    D3DXVECTOR3 pos;            // 位置
}BLOCK_HIT_INFO;

//===========================================
// プロトタイプ宣言
//===========================================
bool OutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool OutGame2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize);
bool SteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize);
bool RectangleCollision2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
bool RectangleCollision3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 *pPos1, D3DXVECTOR3 *pPos1Old, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2);
int GetRandNum(int nMax, int nMin);

#endif