//===============================================
//
// 描画処理 (renderer.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "fade.h"
#include "manager.h"
#include "game.h"
#include "camera.h"
#include "block.h"
#include "enemy.h"
#include "effect3d.h"
#include "CreateStage.h"

//========================================
// レンダリングのデフォルトコンストラクタ
//========================================
CRenderer::CRenderer()
{
	m_nSaveMapJudgeTime = 120;
	m_nBlockSaveDateJudgeTime = 120;
    m_pD3D = NULL;			// Direct3Dオブジェクト
    m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	m_pFont = NULL; // フォントのポインタ
}

//========================================
// レンダリングのデストラクタ
//========================================
CRenderer::~CRenderer()
{

}

//========================================
// レンダリングの初期化処理
//========================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;
    D3DMATERIAL9 material;

    // Direct3Dオブジェクトの作成
    m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (m_pD3D == NULL)
    {
        return E_FAIL;
    }

    // 現在のディスプレイモードを取得
    if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    // デバイスのプレゼンテーションパラメータの設定
    ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
    d3dpp.BackBufferCount = 1;							// バックバッファの数
    d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
    d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
    d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
    d3dpp.Windowed = bWindow;						// ウィンドウモード
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

                                                                // デバイスの生成
                                                                // ディスプレイアダプタを表すためのデバイスを作成
                                                                // 描画と頂点処理をハードウェアで行なう
    if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, &m_pD3DDevice)))
    {
        // 上記の設定が失敗したら
        // 描画をハードウェアで行い、頂点処理はCPUで行なう
        if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp, &m_pD3DDevice)))
        {
            // 上記の設定が失敗したら
            // 描画と頂点処理をCPUで行なう
            if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF, hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &d3dpp, &m_pD3DDevice)))
            {
                // 生成失敗
                return E_FAIL;
            }
        }
    }

    // レンダーステートの設定
    m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);          // カリングをする
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                  // Zバッファを使用
    m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);         // αブレンドを行う
    m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // αソースカラーの指定
    m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);// αデスティネーションカラー
    m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                 // カメラを使用する
    m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00444444);            // アンビエントの設定

                                                                        // サンプラーステートの設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);   // テクスチャU値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);   // テクスチャV値の繰り返し設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);    // テクスチャ拡大時の補間設定
    m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);    // テクスチャ縮小時の補間設定

                                                                            // テクスチャステージステートの設定
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // 最初のアルファ引数(初期値)
    m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // 2番目のアルファ引数(初期値)

                                                                            // マテリアルの設定
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    m_pD3DDevice->SetMaterial(&material);

    // デバッグ情報表示用フォントの生成
    D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);

	D3DXCreateFont(m_pD3DDevice, 40, 40, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pPauseFont);

    return S_OK;
}

//========================================
// レンダリングの終了処理
//========================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
    // デバッグ情報表示用フォントの破棄
    if (m_pFont != NULL)
    {
        m_pFont->Release();
        m_pFont = NULL;
    }
#endif

    // デバイスの破棄
    if (m_pD3DDevice != NULL)
    {
        m_pD3DDevice->Release();
        m_pD3DDevice = NULL;
    }

    // Direct3Dオブジェクトの破棄
    if (m_pD3D != NULL)
    {
        m_pD3D->Release();
        m_pD3D = NULL;
    }
}

//========================================
// レンダリングの更新処理
//========================================
void CRenderer::Update(void)
{
    // 全てのオブジェクトを更新
    CScene::UpdateAll();
}

//========================================
// レンダリングの描画処理
//========================================
void CRenderer::Draw(void)
{
    // バックバッファ＆Ｚバッファのクリア
    m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(249, 102, 71, 255), 1.0f, 0);

    // Direct3Dによる描画の開始
    if (SUCCEEDED(m_pD3DDevice->BeginScene()))
    {
        // 全てのオブジェクトを描画
        CScene::DrawAll();

#ifdef _DEBUG
        // FPS表示
        DrawFPS();
#endif
		DrawPosRAndSelectType();
		DrawBlockPos();
		DrawEnemyPos();
		DrawCursorPos();
		DrawFixedSshaft();
		DrawGreaseJudge();
		DrawSeveMapJudge();
		DrawCopyDate();
		DrawCreateObjectType();
		DrawPauseJudge();
		DrawObjectSaveDate();

        // Direct3Dによる描画の終了
        m_pD3DDevice->EndScene();
    }

    // バックバッファとフロントバッファの入れ替え
    m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
// FPS描画処理
//========================================
#ifdef _DEBUG
void CRenderer::DrawFPS(void)
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    char str[BYTE];

    wsprintf(str, "FPS:%d\n", GetFPS());

    // テキスト描画
    m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif

void CRenderer::DrawPosRAndSelectType(void) {
	RECT rect = { 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
	rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
	char str[BYTE];

	if (CCreateStage::GetCreateObject() == CCreateStage::CREATEOBJECT_STAGE) {
		wsprintf(str, "posR.x:%d posR.y:%d posR.z:%d SelectName:%s\n", (int)CCamera::GetCameraposR().x, 0, (int)CCamera::GetCameraposR().z, CBlock::GetBlockData(CCreateStage::GetBlockType()).DrawName);
	}
	if (CCreateStage::GetCreateObject() == CCreateStage::CREATEOBJECT_ENEMY) {
		wsprintf(str, "posR.x:%d posR.y:%d posR.z:%d SelectName:%s\n", (int)CCamera::GetCameraposR().x, 0, (int)CCamera::GetCameraposR().z, CEnemy::GetEnemyData(CCreateStage::GetEnemyType()).DrawName);
	}

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void CRenderer::DrawBlockPos(void) {
	if (CBlock::GetSelectBlock() != NULL) {
		char str[BYTE];

		RECT rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };

		wsprintf(str, "BlockPos.x:%d BlockPos.y:%d BlockPos.z:%d BlockType:%d BlockNuber:%d\n", (int)CBlock::GetSelectBlock()->GetPos().x, (int)CBlock::GetSelectBlock()->GetPos().y, (int)CBlock::GetSelectBlock()->GetPos().z, (int)CBlock::GetSelectBlock()->GetType(), CBlock::GetSelectBlock()->GetNumber());

#ifndef _DEBUG
		rect = { 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT };

		wsprintf(str, "BlockPos.x:%d BlockPos.y:%d BlockPos.z:%d BlockType:%d BlockNuber:%d\n", (int)CBlock::GetSelectBlock()->GetPos().x, (int)CBlock::GetSelectBlock()->GetPos().y, (int)CBlock::GetSelectBlock()->GetPos().z, (int)CBlock::GetSelectBlock()->GetType(), CBlock::GetSelectBlock()->GetNumber());
#endif

		// テキスト描画
		m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}

void CRenderer::DrawEnemyPos(void) {
	if (CEnemy::GetSelectEnemy() != NULL) {
		char str[BYTE];

		RECT rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };

		wsprintf(str, "EnemyPos.x:%d EnemyPos.y:%d EnemyPos.z:%d EnemyType:%d EnemyNumber:%d\n", (int)CEnemy::GetSelectEnemy()->GetPos().x, (int)CEnemy::GetSelectEnemy()->GetPos().y, (int)CEnemy::GetSelectEnemy()->GetPos().z, (int)CEnemy::GetSelectEnemy()->GetType(), CEnemy::GetSelectEnemy()->GetNumber());

#ifndef _DEBUG
		rect = { 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT };

		wsprintf(str, "EnemyPos.x:%d EnemyPos.y:%d EnemyPos.z:%d EnemyType:%d EnemyNumber:%d\n", (int)CEnemy::GetSelectEnemy()->GetPos().x, (int)CEnemy::GetSelectEnemy()->GetPos().y, (int)CEnemy::GetSelectEnemy()->GetPos().z, (int)CEnemy::GetSelectEnemy()->GetType(), CEnemy::GetSelectEnemy()->GetNumber());

#endif

		// テキスト描画
		m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}

void CRenderer::DrawCursorPos(void) {
	RECT rect = { 0, 60, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
	rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
	char str[BYTE];

	wsprintf(str, "Cursor.x:%d Cursor.y:%d Cursor.z:%d\n", (int)CEffect3D::GetPos().x, (int)CEffect3D::GetPos().y, (int)CEffect3D::GetPos().z);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void CRenderer::DrawCopyDate(void) {
	if (CCreateStage::GetCopynType() >= 0) {
		RECT rect = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
		rect = { 0, 60, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
		char str001[BYTE];
		char str002[BYTE];

		switch (CCreateStage::GetCreateObject()) {
		case CCreateStage::CREATEOBJECT_STAGE:
			wsprintf(str002, "Stage\n");

			wsprintf(str001, "Copypos.y:%d CopyRot.x:%d CopyRot.y:%d CopyRot.z:%d CopynName:%s ObjectType:%s\n", (int)CCreateStage::GetCopyPos().y, (int)D3DXToDegree(CCreateStage::GetCopyPosR().x), (int)D3DXToDegree(CCreateStage::GetCopyPosR().y), (int)D3DXToDegree(CCreateStage::GetCopyPosR().z), CBlock::GetBlockData(CCreateStage::GetCopyType()).DrawName, str002);
			break;
		case CCreateStage::CREATEOBJECT_ENEMY:
			wsprintf(str002, "Enemy\n");

			wsprintf(str001, "Copypos.y:%d CopyRot.x:%d CopyRot.y:%d CopyRot.z:%d CopynName:%s ObjectType:%s\n", (int)CCreateStage::GetCopyPos().y, (int)D3DXToDegree(CCreateStage::GetCopyPosR().x), (int)D3DXToDegree(CCreateStage::GetCopyPosR().y), (int)D3DXToDegree(CCreateStage::GetCopyPosR().z), CEnemy::GetEnemyData(CCreateStage::GetCopyType()).DrawName, str002);
			break;
		}

		// テキスト描画
		m_pFont->DrawText(NULL, str001, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}

void CRenderer::DrawFixedSshaft(void) {
	RECT rect = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
	rect = { 0, 80, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
	char str[BYTE];

	wsprintf(str, "x軸固定:%s, z軸固定:%s", CCreateStage::GetFixedSshaft(0) ? "true" : "false", CCreateStage::GetFixedSshaft(1) ? "true" : "false");

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void CRenderer::DrawGreaseJudge(void) {
	RECT rect = { 0, 120, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
	rect = { 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
	char str[BYTE];

	wsprintf(str, "グリッド:%s", CCreateStage::GetGreaseJudge() ? "true" : "false");

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void CRenderer::DrawSeveMapJudge(void) {
	if (m_nSaveMapJudgeTime >= 0) {
		if (CCreateStage::GetPushedControlS() == true) {
			RECT rect = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
#ifndef _DEBUG
			rect = { 0, 120, SCREEN_WIDTH, SCREEN_HEIGHT };
#endif
			char str[256];

			if (CCreateStage::GetSaveJudge() != true) {
				wsprintf(str, "%s", "テキストファイルが開けないか、\nオブジェクトが設置されていません");
			}
			if (CCreateStage::GetSaveJudge() == true) {
				wsprintf(str, "%s", "テキストファイルへの出力に成功しました");
			}

			// テキスト描画
			m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

			m_nSaveMapJudgeTime--;

			if (m_nSaveMapJudgeTime <= 0) {
				m_nSaveMapJudgeTime = 120;
				CCreateStage::SetSaveJudge(false);
				CCreateStage::SetPushedControlS(false);
			}
		}
	}
}

void CRenderer::DrawObjectSaveDate(void) {
	if (m_nBlockSaveDateJudgeTime >= 0) {
		if (CCreateStage::GetWriteSaveDetaJudge() == true) {
			RECT rect = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
			if (CCreateStage::GetSaveJudge() == true) {
				rect = { 0, 160, SCREEN_WIDTH, SCREEN_HEIGHT };
			}
			else {
				rect = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
			}
#ifndef _DEBUG
			if (CCreateStage::GetSaveJudge() == true) {
				rect = { 0, 140, SCREEN_WIDTH, SCREEN_HEIGHT };
			}
			else {
				rect = { 0, 120, SCREEN_WIDTH, SCREEN_HEIGHT };
			}
#endif

			char str[256];

			wsprintf(str, "%s", "変更内容をdebug.txtに出力しました");

			// テキスト描画
			m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

			m_nBlockSaveDateJudgeTime--;

			if (m_nBlockSaveDateJudgeTime <= 0) {
				m_nBlockSaveDateJudgeTime = 120;
				CCreateStage::SetWriteSaveDataJudge(false);
			}
		}
	}
}

void CRenderer::DrawCreateObjectType(void) {
	RECT rect = { SCREEN_WIDTH / 2 - 20, 20, SCREEN_WIDTH, SCREEN_HEIGHT };

	char str[BYTE];

	switch (CCreateStage::GetCreateObject()) {
	case CCreateStage::CREATEOBJECT_STAGE:
		wsprintf(str, "StageCreateNow\n");
		break;
	case CCreateStage::CREATEOBJECT_ENEMY:
		wsprintf(str, "EnemyCreateNow\n");
		break;
	}

	if (str != NULL) {
		// テキスト描画
		m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}

void CRenderer::DrawPauseJudge(void) {
	RECT rect = { SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 20, SCREEN_WIDTH, SCREEN_HEIGHT };

	char str[BYTE];

	if (CCreateStage::GetPauseJudge() == true) {
		wsprintf(str, "Pause Now\n");

		// テキスト描画
		m_pPauseFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}
}