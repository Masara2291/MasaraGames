//===============================================
//
// 様々な処理をまとめたファイル (library.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "library.h"

//===========================================
// 画面外かどうか(二次元)
//===========================================
bool OutScreen2D(const D3DXVECTOR3 *pPos, const D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bOutScreen = false;    // 画面外かどうか

                                // 画面の端から出ているかどうか
    if (pPos->x < -(pSize->x / 2) ||
        pPos->x > SCREEN_WIDTH + pSize->x / 2 ||
        pPos->y < -(pSize->y / 2) ||
        pPos->y > SCREEN_HEIGHT + pSize->y / 2)
    {
        // 画面外のフラグをtrueに
        bOutScreen = true;
    }

    return bOutScreen;
}

//===========================================
// ゲーム外かどうか(二次元)
//===========================================
bool OutGame2D(const D3DXVECTOR3 * pPos, const D3DXVECTOR3 * pSize)
{
    // 変数宣言
    bool bOutGame = false;    // ゲーム外かどうか

                              // 画面の端から2倍のところから出ているかどうか
    if (pPos->x < -SCREEN_WIDTH - (pSize->x / 2) ||
        pPos->x >(SCREEN_WIDTH * 2) + pSize->x / 2 ||
        pPos->y < -SCREEN_HEIGHT - (pSize->y / 2) ||
        pPos->y >(SCREEN_HEIGHT * 2) + pSize->y / 2)
    {
        // ゲーム外のフラグをtrueに
        bOutGame = true;
    }

    return bOutGame;
}

//===========================================
// 画面内にとどまらせる(二次元)
//===========================================
bool SteyInScreen2D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pSize)
{
    // 変数宣言
    bool bScreenEdge = false;    // 画面端かどうか

                                 // 画面の左
    if (pPos->x < pSize->x / 2)
    {
        // とどまらせる
        pPos->x = pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の右
    if (pPos->x > SCREEN_WIDTH - pSize->x / 2)
    {
        // とどまらせる
        pPos->x = SCREEN_WIDTH - pSize->x / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の上
    if (pPos->y < pSize->y / 2)
    {
        // とどまらせる
        pPos->y = pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    // 画面の下
    if (pPos->y > SCREEN_HEIGHT - pSize->y / 2)
    {
        // とどまらせる
        pPos->y = SCREEN_HEIGHT - pSize->y / 2;

        // 画面端のフラグをtrueに
        bScreenEdge = true;
    }

    return bScreenEdge;
}

//===========================================
// 矩形の当たり判定2D
//===========================================
bool RectangleCollision2D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;    // 当たったかどうか

                          // 二つのものの当たり判定を計算
    if (pPos1->x - pSize1->x / 2 <= pPos2->x + pSize2->x / 2 &&
        pPos1->x + pSize1->x / 2 >= pPos2->x - pSize2->x / 2 &&
        pPos1->y - pSize1->y / 2 <= pPos2->y + pSize2->y / 2 &&
        pPos1->y + pSize1->y / 2 >= pPos2->y - pSize2->y / 2)
    {
        // 当たったフラグをtrueに
        bHit = true;
    }

    return bHit;
}

//===========================================
// 矩形の当たり判定3D
//===========================================
bool RectangleCollision3D(const D3DXVECTOR3 *pPos1, const D3DXVECTOR3*pPos2, const D3DXVECTOR3*pSize1, const D3DXVECTOR3*pSize2)
{
    // 変数宣言
    bool bHit = false;  // 当たったかどうか

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;    //ぶつかる側の最大値
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, pSize1->y * (-1), -pSize1->z / 2) + *pPos1;       //ぶつかる側の最小値
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;    //ぶつかられる側の最大値
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;       //ぶつかられる側の最小値

                                                                                            // 当たり判定を計算
    if (box1Max.y > box2Min.y&&
        box1Min.y < box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        bHit = true;
    }

    return bHit;
}

//===========================================
// ブロックの当たり判定3D
//===========================================
BLOCK_HIT_INFO BlockCollision3D(D3DXVECTOR3 * pPos1, D3DXVECTOR3 * pPos1Old, const D3DXVECTOR3 * pPos2, const D3DXVECTOR3 * pSize1, const D3DXVECTOR3 * pSize2)
{
    // ブロックに当たった情報
    BLOCK_HIT_INFO blockHitInfo;
    blockHitInfo.hitSurface = HIT_SURFACE_NONE;
    blockHitInfo.pos = *pPos1;

    D3DXVECTOR3 box1Max = D3DXVECTOR3(pSize1->x / 2, pSize1->y, pSize1->z / 2) + *pPos1;      //ぶつかる側の最大値
    D3DXVECTOR3 box1Min = D3DXVECTOR3(-pSize1->x / 2, 0.0f, -pSize1->z / 2) + *pPos1;         //ぶつかる側の最小値
    D3DXVECTOR3 box2Max = D3DXVECTOR3(pSize2->x / 2, pSize2->y, pSize2->z / 2) + *pPos2;      //ぶつかられる側の最大値
    D3DXVECTOR3 box2Min = D3DXVECTOR3(-pSize2->x / 2, 0.0f, -pSize2->z / 2) + *pPos2;         //ぶつかられる側の最小値

                                                                                              // 当たり判定を計算
    if (box1Max.y > box2Min.y&&
        box1Min.y <= box2Max.y&&
        box1Max.x > box2Min.x&&
        box1Min.x < box2Max.x&&
        box1Max.z > box2Min.z&&
        box1Min.z < box2Max.z)
    {
        if (box1Max.y > box2Min.y&&
            pPos1Old->y + pSize1->y / 2 < box2Min.y)
        {// Y軸の下
            blockHitInfo.hitSurface = HIT_SURFACE_BOTTOM;
            blockHitInfo.pos.y = box2Min.y - pSize1->y;
        }
        if (box1Min.y <= box2Max.y&&
            pPos1Old->y >= box2Max.y)
        {// Y軸の上
            blockHitInfo.hitSurface = HIT_SURFACE_TOP;
            blockHitInfo.pos.y = box2Max.y;
        }
        if (box1Max.x > box2Min.x&&
            pPos1Old->x + pSize1->x / 2 <= box2Min.x)
        {// X軸の左
            blockHitInfo.hitSurface = HIT_SURFACE_LEFT;
            blockHitInfo.pos.x = box2Min.x - pSize1->x / 2;
        }
        if (box1Min.x < box2Max.x&&
            pPos1Old->x - pSize1->x / 2 >= box2Max.x)
        {// X軸の右
            blockHitInfo.hitSurface = HIT_SURFACE_RIGHT;
            blockHitInfo.pos.x = box2Max.x + pSize1->x / 2;
        }
        if (box1Max.z > box2Min.z&&
            pPos1Old->z + pSize1->z / 2 <= box2Min.z)
        {// Z軸の手前
            blockHitInfo.hitSurface = HIT_SURFACE_FRONT;
            blockHitInfo.pos.z = box2Min.z - pSize1->z / 2;
        }
        if (box1Min.z < box2Max.z&&
            pPos1Old->z - pSize1->z / 2 >= box2Max.z)
        {// Z軸の奥
            blockHitInfo.hitSurface = HIT_SURFACE_BACK;
            blockHitInfo.pos.z = box2Max.z + pSize1->z / 2;
        }
    }

    return blockHitInfo;
}

//===========================================
// 範囲内で、ランダムな数字を得る
//===========================================
int GetRandNum(int nMax, int nMin)
{
    int nRandNum = rand() % (nMax - nMin + 1) + nMin;

    return nRandNum;
}
