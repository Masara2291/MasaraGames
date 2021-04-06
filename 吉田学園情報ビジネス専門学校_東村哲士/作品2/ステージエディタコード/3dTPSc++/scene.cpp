//===============================================
//
// シーン上のオブジェクト処理 (scene.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "scene.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================
int CScene::m_nNumAll = 0;

CScene *CScene::m_pTop = NULL;
CScene *CScene::m_pCur = NULL;

//========================================
// シーン上のオブジェクトのデフォルトコンストラクタ
//========================================
CScene::CScene()
{
    // 先頭がないなら、先頭に
    if (m_pTop == NULL)
    {
        m_pTop = this;
    }

    // 現在における最新のオブジェクトがないなら、最新に
    if (m_pCur == NULL)
    {
        m_pCur = this;
    }

    // 現在のオブジェクトの次のオブジェクトを、自分にする
    m_pCur->m_pNext = this;

    // 現在のオブジェクトが自分の場合
    if (m_pCur == this)
    {
        // 自分の前のオブジェクトを、NULLにする
        m_pPrev = NULL;
    }
    else
    {
        // 自分の前のオブジェクトを、現在のオブジェクトにする
        m_pPrev = m_pCur;
    }

    // 現在のオブジェクトを、自分にする
    m_pCur = this;

    // 自分の次のオブジェクトを、NULLにする
    m_pNext = NULL;

    // 増えたオブジェクトをカウント
    m_nNumAll++;

    // オブジェクトの種類をNULLにする
    m_objType = OBJTYPE_NONE;

    // 使用するフラグをtrueに
    m_bUse = true;
}

//========================================
// シーン上のオブジェクトのオーバーライドされたコンストラクタ
//========================================
CScene::CScene(OBJTYPE objType)
{
    // 先頭がないなら、先頭に
    if (m_pTop == NULL)
    {
        m_pTop = this;
    }

    // 現在における最新のオブジェクトがないなら、最新に
    if (m_pCur == NULL)
    {
        m_pCur = this;
    }

    // 現在のオブジェクトの次のオブジェクトを、自分にする
    m_pCur->m_pNext = this;

    // 現在のオブジェクトが自分の場合
    if (m_pCur == this)
    {
        // 自分の前のオブジェクトを、NULLにする
        m_pPrev = NULL;
    }
    else
    {
        // 自分の前のオブジェクトを、現在のオブジェクトにする
        m_pPrev = m_pCur;
    }

    // 現在のオブジェクトを、自分にする
    m_pCur = this;

    // 自分の次のオブジェクトを、NULLにする
    m_pNext = NULL;

    // 増えたオブジェクトをカウント
    m_nNumAll++;

    // オブジェクトの種類を引数と結びつける
    m_objType = objType;

    // 使用するフラグをtrueに
    m_bUse = true;
}

//========================================
// シーン上のオブジェクトのデストラクタ
//========================================
CScene::~CScene()
{

}

//==============================================
// シーン上のオブジェクトを全て更新
//==============================================
void CScene::UpdateAll(void)
{
    // 先頭、最新のものがあるなら
    if (m_pTop != NULL && m_pCur != NULL)
    {
        // 記憶用の変数
        CScene*pScene = m_pTop;

        do
        {
            // 記憶用の変数(Update中に、Uninitされることを考慮)
            CScene*pNextScene = pScene->m_pNext;

            // 更新処理
            pScene->Update();

            // 使用フラグがfalseなら
            if (pScene->m_bUse == false)
            {
                // メモリの開放
                //delete pScene;
                pScene = NULL;
            }

            // 次のシーンに変えていく
            pScene = pNextScene;

        } while (pScene != NULL);
    }
}

//==============================================
// シーン上のオブジェクトを全て描画
//==============================================
void CScene::DrawAll(void)
{
    // 先頭、最新のものがあるなら
    if (m_pTop != NULL && m_pCur != NULL)
    {
        // 変数宣言
        CScene *pScene = m_pTop;                        // シーンのポインタ記憶用
        CScene**apSceneOrder = new CScene*[m_nNumAll];  // 描画の順番を変えるため、配列に情報を記憶

                                                        // 配列の初期化
        for (int nCntObject = 0; nCntObject < m_nNumAll; nCntObject++)
        {
            // シーン上のオブジェクトの情報を記憶
            apSceneOrder[nCntObject] = pScene;

            // シーンのポインタを進める
            pScene = pScene->m_pNext;
        }

        // バブルソートは遅い
        //BubbleSort(&apSceneOrder[0]);

        // クイックソートで配列を、描画順に並び替える
        QuickSort(&apSceneOrder[0], 0, m_nNumAll - 1);

        // デバッグ用
        int nNullData = 0;

        // 順番に描画
        for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
        {
            if (apSceneOrder[nCnt] != NULL)
            {
                apSceneOrder[nCnt]->Draw();

                nNullData = m_nNumAll;
            }
            else
            {
                nNullData = nCnt;
            }
        }

        // 用意した配列の消去
        delete[] apSceneOrder;
    }
}

//==============================================
// バブルソート
//==============================================
void CScene::BubbleSort(CScene * apScene[])
{
    // 描画順をソート(選択法)を使って、タイプ順に並び替える（昇順）
    for (int nNum01 = 0; nNum01 < m_nNumAll - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < m_nNumAll; nNum02++)
        {
            // どちらも中身があるなら
            if (apScene[nNum01] != NULL && apScene[nNum02] != NULL)
            {
                // 若い番号のタイプのほうが大きいなら
                if (apScene[nNum01]->m_objType > apScene[nNum02]->m_objType)
                {
                    // 若い番号を記憶
                    CScene *pScene = apScene[nNum01];

                    // 若い番号に次の番号を代入
                    apScene[nNum01] = apScene[nNum02];

                    // 次の番号に記憶したものを代入
                    apScene[nNum02] = pScene;
                }
            }
        }
    }
}

//==============================================
// クイックソート
//==============================================
void CScene::QuickSort(CScene * apScene[], int left, int right)
{
    // 変数定義
    int Left;
    int Right;
    CScene* pivot;
    int nPivotAnswer = (left + right) / 2;

    // 初期値は引数から
    Left = left;
    Right = right;

    // ピボットが奇数なら、繰り下げる
    if (nPivotAnswer % 2 != 0)
    {
        nPivotAnswer = nPivotAnswer - (1 / 2);
    }

    // ピボットの範囲を設定(下限)
    if (nPivotAnswer < 0)
    {
        nPivotAnswer = 0;
    }

    // ピボットの範囲を設定(上限)
    if (nPivotAnswer > m_nNumAll - 1)
    {
        nPivotAnswer = m_nNumAll - 1;
    }

    // 基準は真ん中に設定
    pivot = apScene[nPivotAnswer];

    // ソーティング
    while (1)
    {
        // 基準より小さい値を左から見つけていく
        while (apScene[Left]->m_objType < pivot->m_objType)
        {
            Left++;

            // 最大値の制限
            if (Left > m_nNumAll - 1)
            {
                Left = m_nNumAll - 1;

                break;
            }
        }

        // 基準より大きい値を右から見つけていく
        while (pivot->m_objType < apScene[Right]->m_objType)
        {
            Right--;

            // 最小値の制限
            if (Right < 0)
            {
                Right = 0;

                break;
            }
        }

        // 見つかった値の順序が逆になったら終了 
        if (Left >= Right)
        {
            break;
        }

        // 交換処理
        if (apScene[Left] != NULL && apScene[Right] != NULL)
        {
            // 若い番号を記憶
            CScene *pScene = apScene[Left];

            // 若い番号に次の番号を代入
            apScene[Left] = apScene[Right];

            // 次の番号に記憶したものを代入
            apScene[Right] = pScene;
        }

        // 次の値に移動
        Left++;
        Right--;

        // 最大値の制限
        if (Left > m_nNumAll - 1)
        {
            break;
        }

        // 最小値の制限
        if (Right < 0)
        {
            break;
        }
    }

    //左のデータ群を対象としてクイックソートを再帰
    if (left < Left - 1)
    {
        QuickSort(apScene, left, Left - 1);
    }

    //右のデータ群を対象としてクイックソートを再帰
    if (Right + 1 < right)
    {
        QuickSort(apScene, Right + 1, right);
    }
}

//==============================================
// シーン上のオブジェクトを全て開放
//==============================================
void CScene::ReleaseAll(void)
{
    // 先頭、最新のものがあるなら
    if (m_pTop != NULL && m_pCur != NULL)
    {
        // 記憶用の変数
        CScene*pScene = m_pTop;

        do
        {
            // 記憶用の変数
            CScene*pNextScene = pScene->m_pNext;

            // 終了処理
            pScene->Uninit();

            // 使用フラグがfalseなら
            if (pScene->m_bUse == false)
            {
                // メモリの開放
                //delete pScene;
                pScene = NULL;
            }

            // 次のシーンに変えていく
            pScene = pNextScene;

        } while (pScene != NULL);
    }
}

//==============================================
// シーン上のオブジェクトを全て開放
//==============================================
void CScene::Release(void)
{
    // 中身があるなら、
    if (this != NULL)
    {
        // 先頭なら
        if (this == m_pTop)
        {
            // 次の中身があるなら
            if (m_pNext != NULL)
            {
                // 次のものの前の情報を、NULLに
                m_pNext->m_pPrev = NULL;

                // それを先頭に
                m_pTop = m_pNext;
            }
            else
            {
                // 先頭の次がないなら、先頭と現在を示すポインタをNULLに
                m_pTop = NULL;
                m_pCur = NULL;
            }
        }
        // 先頭でないなら
        else
        {
            // 次の中身があるなら
            if (m_pNext != NULL)
            {
                // 前のものの次の情報を、自身の次の情報に
                m_pPrev->m_pNext = m_pNext;

                // 次のものの前の情報を、自身の前の情報に
                m_pNext->m_pPrev = m_pPrev;
            }
            else
            {
                // 前のものの次の情報を、なくす
                m_pPrev->m_pNext = NULL;

                // 現在の最新のものを、前の情報に
                m_pCur = m_pPrev;
            }
        }

        // メモリの開放
        delete this;

        // 使用フラグをfalseに
        m_bUse = false;

        // 総数を減らす
        m_nNumAll--;
    }
}
