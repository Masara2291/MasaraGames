//====================================================================
//
// シーン上のオブジェクト処理 (scene.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// マクロ定義
//================================================

//================================================
// クラス宣言
//================================================

// オブジェクトのクラス
class CScene
{
public:
    // オブジェクトの種類(描画順)
	typedef enum
	{
		OBJTYPE_NONE,      // なし
		OBJTYPE_BLOCK,     // ブロック
		OBJTYPE_BULLET,    // 弾
		OBJTYPE_PLAYER,    // プレイヤー
		OBJTYPE_DEBUG,     // デバッグ
		OBJTYPE_EFFECT3D,  // エフェクト3D
		OBJTYPE_BG,        // 背景
		OBJTYPE_UI,        // UI
		OBJTYPE_GAUGE,     // ゲージ
		OBJTYPE_EFFECT,    // エフェクト
		OBJTYPE_ENEMY,     // 敵
        OBJTYPE_MAX        // オブジェクトの種類の最大数
    }OBJTYPE;

    CScene();
    CScene(OBJTYPE objType);    // イニシャライザでオブジェクトタイプを紐づけ
    virtual ~CScene();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot) = 0;
    virtual void Uninit(void) = 0;
    virtual void Update(void) = 0;
    virtual void Draw(void) = 0;
    static void UpdateAll(void);                                    // 全て更新
    static void DrawAll(void);                                      // 全て描画
    static void BubbleSort(CScene*apScene[]);                       // バブルソート
    static void QuickSort(CScene*apScene[], int left, int right);   // クイックソート
    static void ReleaseAll(void);                                   // 全て開放
                                                                    //static void ReleaseStage(void);                               // 全ての背景、敵、ブロック、アイテムを開放
    OBJTYPE GetObjType(void) { return m_objType; }                  // オブジェクトタイプを取得

    static int GetNumAll(void) { return m_nNumAll; }                 // 全体の数を取得
    static CScene* GetTopScene(void) { return m_pTop; }              // 先頭のオブジェクトを取得
	static CScene* GetCurScene(void) { return m_pCur; }              // 現在のオブジェクトを取得
    CScene* GetNextScene(void) { return m_pNext; }                   // 次のオブジェクトを取得

    void SetObjType(OBJTYPE objtype) { m_objType = objtype; }

protected:
    void Release(void);                 // 単体を開放

private:
    static CScene *m_pTop;  // 先頭のオブジェクトへのポインタ
    static CScene *m_pCur;  // 現在における最新のオブジェクトへのポインタ
    CScene *m_pPrev;        // 前のオブジェクトへのポインタ
    CScene *m_pNext;        // 次のオブジェクトへのポインタ

    static int m_nNumAll;   // シーン上のオブジェクトの最大数
    OBJTYPE m_objType;      // オブジェクトの種類

    bool m_bUse;            // 使用するかどうか
};

#endif