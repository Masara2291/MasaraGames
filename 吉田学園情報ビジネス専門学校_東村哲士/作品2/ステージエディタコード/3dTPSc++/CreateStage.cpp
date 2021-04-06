#define _CRT_SECURE_NO_WARNINGS

#include "CreateStage.h"

#include "block.h"
#include "enemy.h"
#include "field.h"
#include "camera.h"
#include "effect3d.h"
#include "library.h"

bool CCreateStage::m_bFixedSshaft[2]                                           = { false, false };
bool CCreateStage::m_bGreaseJudge                                              = false;
bool CCreateStage::m_bSaveJudge                                                = false;
bool CCreateStage::m_bPushedControlS                                           = false;
bool CCreateStage::m_bUseAllDelete                                             = false;
bool CCreateStage::m_WriteSaveDate                                             = false;
int CCreateStage::m_nCopyType                                                  = -1;
D3DXVECTOR3 CCreateStage::m_CopyRot                                            = {};
D3DXVECTOR3 CCreateStage::m_CopyPos                                            = {};
CCreateStage::BLOCKSAVEDATE CCreateStage::m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM] = {};
CCreateStage::CREATEOBJECT CCreateStage::m_CreateType                          = CREATEOBJECT_STAGE;
D3DXVECTOR3 CCreateStage::m_FieldSiz                                           = {};
char CCreateStage::m_cTxtName[3][256]                                          = {};
int CCreateStage::m_nBlockType                                                 = 0;
int CCreateStage::m_nEnemyType                                                 = 0;
bool CCreateStage::m_bPauseJudge                                               = false;

CCreateStage::CCreateStage()
{
    for(int nCount = 0; nCount < BLOCK_SAVE_MAX_NUM; nCount++) {
        m_aBlockSaveDate[nCount].DetailsChange = DETAILSCHANGE_NULL;
        m_aBlockSaveDate[nCount].bChangeJudge  = false;
    }

	LoadTextName();
}

CCreateStage::~CCreateStage()
{
}

HRESULT CCreateStage::Init(void)
{
    m_nBlockType = 0;
    m_nEnemyType = 0;

    return S_OK;
}

void CCreateStage::Uninit(void)
{
}

void CCreateStage::Update(void)
{
    static int nCount = 0;
    m_nLastCountPos   = 0;
    m_nLastCountRot   = 0;
    m_posR            = CCamera::GetCameraposR();
    //キーボードの確保したメモリを取得
    m_pInputKeyboard = CManager::GetInputKeyboard();
    //マウスの確保したメモリを取得
    m_pMouse = CManager::GetMouse();

	// 左コントロールが押されていない場合
	if (!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && !m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL)) {
		// Pキーでポーズ画面にする
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_P)) {
			// ポーズ画面使用の変数をトリガーする
			m_bPauseJudge = !m_bPauseJudge;

			// trueの場合
			if (m_bPauseJudge == true) {
				// マウスカーソルの非表示
				ShowCursor(TRUE);
			}
			// falseの場合
			else if (m_bPauseJudge == false) {
				// マウスカーソルの非表示
				ShowCursor(FALSE);
			}
		}
	}

	if (m_bPauseJudge == false) {
		// 変更内容がcontrol + Zの時に変更されたものの数を数える
		CountDetallsChangeLAST();

		// F3/F4キーで設置するタイプを切り替える
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F3)) {
			m_CreateType = CREATEOBJECT_STAGE;
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F4)) {
			m_CreateType = CREATEOBJECT_ENEMY;
		}

		// F3/F4キーが押された場合、コピーしてる情報を初期化する
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_F3) || m_pInputKeyboard->GetKeyboardTrigger(DIK_F4)) {
			InitCopyDate();
		}

		// 左コントロールとBキーが押されたとき変更前の保存されている情報をテキストファイルに出力する
		if (m_pInputKeyboard->GetKeyboardPress(DIK_B) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_B)) {

			m_WriteSaveDate = true;
			WriteBlockSaveDate(nCount);
		}

		// 左コントロールが押されていない場合
		if (!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && !m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL)) {
			// Zキーが押されたとき
			if (m_pInputKeyboard->GetKeyboardTrigger(DIK_Z)) {
				// bool型変数をトグルさせる
				m_bFixedSshaft[1] = !m_bFixedSshaft[1];
				m_FixedSshaft.z = m_posR.z;
				if (CBlock::GetSelectBlock() != NULL) {
					m_FixedSshaft.z = CBlock::GetSelectBlock()->GetPos().z;
				}
				if (m_bGreaseJudge == true) {
					m_FixedSshaft.z = m_CursorPos.z;
				}
			}

			// Xキーが押されたとき
			if (m_pInputKeyboard->GetKeyboardTrigger(DIK_X)) {
				// bool型変数をトグルさせる
				m_bFixedSshaft[0] = !m_bFixedSshaft[0];
				m_FixedSshaft.x = m_posR.x;
				if (CBlock::GetSelectBlock() != NULL) {
					m_FixedSshaft.x = CBlock::GetSelectBlock()->GetPos().x;
				}
				if (m_bGreaseJudge == true) {
					m_FixedSshaft.x = m_CursorPos.x;
				}
			}
		}

		// Gキーでグリッドの使用を切り替える
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_G)) {
			m_bGreaseJudge = !m_bGreaseJudge;
		}

		// 左コントロールとYキーが押された場合
		if (m_pInputKeyboard->GetKeyboardPress(DIK_Y) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_Y)) {

			// 過去の情報を元に変更後に進める
			nCount = RedoUndoOperation(nCount);
		}
		// 左コントロールとZキーが押された場合
		if (m_pInputKeyboard->GetKeyboardPress(DIK_Z) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_Z)) {

			// 過去の情報を元に変更前に戻す
			nCount = UndoingOperation(nCount);
		}

		// 設置するタイプによって更新処理を切り替える
		switch (m_CreateType) {
		case CREATEOBJECT_STAGE:
			nCount = SetObject(m_pInputKeyboard, m_posR, m_pMouse, nCount, m_aBlockSaveDate[nCount], m_CreateType);
			break;

		case CREATEOBJECT_ENEMY:
			nCount = SetEnemy(m_pInputKeyboard, m_posR, m_pMouse, nCount, m_aBlockSaveDate[nCount], m_CreateType);
			break;
		}

		// グリッドが有効の場合、100マス区切りでカーソルを設置する
		if (m_bGreaseJudge == true) {
			while ((int)m_posR.z % 100 != 0) {
				(int)m_posR.z--;
			}
			while ((int)m_posR.x % 100 != 0) {
				(int)m_posR.x--;
			}

			m_CursorPos = m_posR;
		}

		// グリッドの使用状態に合わせてカーソルを制御する
		if (m_bGreaseJudge == false) {
			CursorControl(m_FixedSshaft, m_posR, m_CreateType);
		}
		if (m_bGreaseJudge == true) {
			CursorControl(m_FixedSshaft, m_CursorPos, m_CreateType);
		}

		// 「現在に変更内容が保存されている場合かつ」、「現在に全消去以外の変更をした場合」かつ、「未来に全消去の変更が行われていた」場合、現在移行行われていた変更内容を初期化する
		if (m_aBlockSaveDate[nCount].DetailsChange != DETAILSCHANGE_NULL && m_aBlockSaveDate[nCount].DetailsChange != DETAILSCHANGE_TOTALELIMINATION && m_aBlockSaveDate[nCount + 1].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
			InitArray(nCount + NEXT_DATA);
		}
		// 現在の情報から変更された時かつ、変更内容が適応されていない場合又はカウント用変数を進める
		if (m_aBlockSaveDate[nCount].DetailsChange != DETAILSCHANGE_NULL && m_aBlockSaveDate[nCount].bChangeJudge == false) {
			nCount++;
		}
	}
}

//=============================================================================
// オブジェクトを設置
//=============================================================================
int CCreateStage::SetObject(CInputKeyboard *m_pInputKeyboard,
                            D3DXVECTOR3 m_posR,
                            CMouse *m_pMouse,
                            int nCount,
                            BLOCKSAVEDATE m_aBlockSaveDate,
                            CREATEOBJECT m_CreateType)
{
    static D3DXVECTOR3 Pos;
    static D3DXVECTOR3 Rot;
    static int nNumber = -1;

    // タイプを管理する変数が範囲外にならないように処理
    if(m_nBlockType >= CBlock::TYPE_MAX) {
        m_nBlockType = CBlock::TYPE_MAX - 1;
    }
    if(m_nBlockType < 0) {
        m_nBlockType = 0;
    }

    //=================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
    // 設置オブジェクトの変更

    // 左コントロールが押されていない場合
    if(!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
        // メイン数字キー
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_1)) {
			SetObjectType(CBlock::TYPE_1);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_2)) {
			SetObjectType(CBlock::TYPE_2);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_3)) {
			SetObjectType(CBlock::TYPE_3);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_4)) {
			SetObjectType(CBlock::TYPE_4);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_5)) {
			SetObjectType(CBlock::TYPE_5);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_6)) {
			SetObjectType(CBlock::TYPE_6);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_7)) {
			SetObjectType(CBlock::TYPE_7);
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_8)) {
			SetObjectType(CBlock::TYPE_8);
		}
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_9)) {
			SetObjectType(CBlock::TYPE_9);
		}
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_0)) {
			SetObjectType(CBlock::TYPE_10);
		}
		// テンキー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD1)) {
			SetObjectType(CBlock::TYPE_11);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD2)) {
			SetObjectType(CBlock::TYPE_12);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD3)) {
			SetObjectType(CBlock::TYPE_13);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD4)) {
			SetObjectType(CBlock::TYPE_14);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD5)) {
			SetObjectType(CBlock::TYPE_15);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD6)) {
			SetObjectType(CBlock::TYPE_16);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD7)) {
			SetObjectType(CBlock::TYPE_17);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD8)) {
			SetObjectType(CBlock::TYPE_18);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD9)) {
			SetObjectType(CBlock::TYPE_19);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0)) {
			SetObjectType(CBlock::TYPE_20);
		}
    }
    // 左コントロールが押されている場合
    else if(m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
		// メイン数字キー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_1)) {
			SetObjectType(CBlock::TYPE_21);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_2)) {
			SetObjectType(CBlock::TYPE_22);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_3)) {
			SetObjectType(CBlock::TYPE_23);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_4)) {
			SetObjectType(CBlock::TYPE_24);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_5)) {
			SetObjectType(CBlock::TYPE_25);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_6)) {
			SetObjectType(CBlock::TYPE_26);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_7)) {
			SetObjectType(CBlock::TYPE_27);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_8)) {
			SetObjectType(CBlock::TYPE_28);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_9)) {
			SetObjectType(CBlock::TYPE_29);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_0)) {
			SetObjectType(CBlock::TYPE_30);
		}
		// テンキー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD1)) {
			SetObjectType(CBlock::TYPE_31);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD2)) {
			SetObjectType(CBlock::TYPE_32);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD3)) {
			SetObjectType(CBlock::TYPE_33);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD4)) {
			SetObjectType(CBlock::TYPE_34);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD5)) {
			SetObjectType(CBlock::TYPE_35);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD6)) {
			SetObjectType(CBlock::TYPE_36);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD7)) {
			SetObjectType(CBlock::TYPE_37);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD8)) {
			SetObjectType(CBlock::TYPE_38);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD9)) {
			SetObjectType(CBlock::TYPE_39);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0)) {
			SetObjectType(CBlock::TYPE_40);
		}
    }
    //=================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

    // 左コントロールとXキーが押されたとき選択中のオブジェクト情報を保存し、選択中のオブジェクトを消す
    if(m_pInputKeyboard->GetKeyboardPress(DIK_X) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_X)) {

        // オブジェクトを選択している場合
        if(CBlock::GetSelectBlock() != NULL) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // 消すオブジェクト情報を保存
            SaveDate(CBlock::GetSelectBlock()->GetPos(),
                     CBlock::GetSelectBlock()->GetRot(),
                     CBlock::GetSelectBlock()->GetNumber(),
                     DETAILSCHANGE_BREAK,
                     false,
                     CBlock::GetSelectBlock()->GetType(),
                     nCount,
                     CScene::OBJTYPE_BLOCK);
            // 選択中のオブジェクト情報を保存
            SetCopyDate(CBlock::GetSelectBlock()->GetPos(),
                        CBlock::GetSelectBlock()->GetRot(),
                        CBlock::GetSelectBlock()->GetType());
            // 選択中のオブジェクトを消す
            CBlock::GetSelectBlock()->Uninit();
        }
    }

    // コントロールとPキーが押された場合
    if(m_pInputKeyboard->GetKeyboardPress(DIK_P) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_P)) {

        if(CBlock::GetBlockNumAll() >= OBJECT_MIN_NUM) {
            // 設置されているオブジェクトを全て消去する関数を読み込む状態に変更
            m_bUseAllDelete = true;
            // 全消去実行直前のオブジェクトの総数を取得
            m_nAllDeleteNum = CBlock::GetBlockNumAll();
        }
    }

    // オブジェクトを選択出来ている場合
    if(CBlock::GetSelectBlock() != NULL) {
        // 変更前の情報を取得
        if(nNumber == -1 || nNumber != CBlock::GetSelectBlock()->GetNumber()) {
            Pos     = CBlock::GetSelectBlock()->GetPos();
            Rot     = CBlock::GetSelectBlock()->GetRot();
            nNumber = CBlock::GetSelectBlock()->GetNumber();
        }
        // 選択中のオブジェクト情報を取得
        D3DXVECTOR3 SelectBlockPos = CBlock::GetSelectBlock()->GetPos();
        D3DXVECTOR3 SelectBlockRot = CBlock::GetSelectBlock()->GetRot();
		
        // Q/Eキーで角度を90度毎に変化させる
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_Q)) {
            nAddupAngle += 90;
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_E)) {
            nAddupAngle -= 90;
        }

        // 左コントロールが押されていない場合
        if(!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            // U/Jキーで高差を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_U)) {
                SelectBlockPos.y += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_J)) {
                SelectBlockPos.y -= 15;
            }
            // I/KキーでX軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_I)) {
                SelectBlockPos.x += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_K)) {
                SelectBlockPos.x -= 15;
            }
            // O/Lキー軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_O)) {
                SelectBlockPos.z += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_L)) {
                SelectBlockPos.z -= 15;
            }
        }
        // 左コントロールが押されている場合
        else if(m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            // U/Jキーで高差を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_U)) {
                SelectBlockPos.y += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_J)) {
                SelectBlockPos.y -= 5;
            }
            // I/KキーでX軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_I)) {
                SelectBlockPos.x += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_K)) {
                SelectBlockPos.x -= 5;
            }
            // O/Lキー軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_O)) {
                SelectBlockPos.z += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_L)) {
                SelectBlockPos.z -= 5;
            }
        }

        // F1/F2キーで選択中オブジェクトの情報を設置された状態に戻す
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_F1)) {
            CBlock::GetSelectBlock()->SetPos(D3DXVECTOR3(Pos.x, Pos.y, Pos.z));
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_F2)) {
            CBlock::GetSelectBlock()->SetRot(D3DXVECTOR3(Rot.x, Rot.y, Rot.z));
        }

        // 左コントロールとCキーが押された場合
        if(m_pInputKeyboard->GetKeyboardPress(DIK_C) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_C)) {
			// コピーするタイプを変数に保存する
			m_nCopyType = CBlock::GetSelectBlock()->GetType();

            // 選択中のオブジェクト情報を保存する
            SetCopyDate(SelectBlockPos, SelectBlockRot, m_nCopyType);
        }

        //　加算/減算させる変数が大きすぎないように調整
        if(nAddupAngle % 360 == 0) {
            nAddupAngle = 0;
        }
        if(SelectBlockPos.y > 10000) {
            SelectBlockPos.y = 10000;
        }
        if(SelectBlockPos.y < -10000) {
            SelectBlockPos.y = -10000;
        }

        // Q/Eキーが押された場合
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_Q) || m_pInputKeyboard->GetKeyboardTrigger(DIK_E)) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // オブジェクトの向きを1桁目切り捨てする
            int RotX = (int)SelectBlockRot.x / 10 * 10;
            int RotY = (int)SelectBlockRot.y / 10 * 10;
            int RotZ = (int)SelectBlockRot.z / 10 * 10;
            // 変更情報を保存する
            SaveDate(CBlock::GetSelectBlock()->GetPos(),
                     CBlock::GetSelectBlock()->GetRot(),
                     CBlock::GetSelectBlock()->GetNumber(),
                     DETAILSCHANGE_CHANGE_ROT,
                     false,
                     CBlock::GetSelectBlock()->GetType(),
                     nCount,
                     CScene::OBJTYPE_BLOCK);

            // 選択中のオブジェクトの向きを変更
            CBlock::GetSelectBlock()->SetRot(
              D3DXVECTOR3(D3DXToRadian(RotX), D3DXToRadian(RotY + nAddupAngle), D3DXToRadian(RotZ)));
        }
        // 左コントロールが押されていない場合
        if(!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_U) || m_pInputKeyboard->GetKeyboardTrigger(DIK_J) || m_pInputKeyboard->GetKeyboardTrigger(DIK_I) || m_pInputKeyboard->GetKeyboardTrigger(DIK_K) || m_pInputKeyboard->GetKeyboardTrigger(DIK_O) || m_pInputKeyboard->GetKeyboardTrigger(DIK_L)) {
                // 配列の確認
                nCount = ShiftArray(nCount);
                // 変更情報を保存する
                SaveDate(CBlock::GetSelectBlock()->GetPos(),
                         CBlock::GetSelectBlock()->GetRot(),
                         CBlock::GetSelectBlock()->GetNumber(),
                         DETAILSCHANGE_CHANGE_POS,
                         false,
                         CBlock::GetSelectBlock()->GetType(),
                         nCount,
                         CScene::OBJTYPE_BLOCK);

                // 選択中のオブジェクトの座標を変更
                CBlock::GetSelectBlock()->SetPos(
                  D3DXVECTOR3(SelectBlockPos.x, SelectBlockPos.y, SelectBlockPos.z));
            }
        }
        else if(m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            if(m_pInputKeyboard->GetKeyboardPress(DIK_U) || m_pInputKeyboard->GetKeyboardPress(DIK_J) || m_pInputKeyboard->GetKeyboardPress(DIK_I) || m_pInputKeyboard->GetKeyboardPress(DIK_K) || m_pInputKeyboard->GetKeyboardPress(DIK_O) || m_pInputKeyboard->GetKeyboardPress(DIK_L)) {
                // 配列の確認
                nCount = ShiftArray(nCount);
                // 変更情報を保存する
                SaveDate(CBlock::GetSelectBlock()->GetPos(),
                         CBlock::GetSelectBlock()->GetRot(),
                         CBlock::GetSelectBlock()->GetNumber(),
                         DETAILSCHANGE_CHANGE_POS,
                         false,
                         CBlock::GetSelectBlock()->GetType(),
                         nCount,
                         CScene::OBJTYPE_BLOCK);

                // 選択中のオブジェクトの座標を変更
                CBlock::GetSelectBlock()->SetPos(
                  D3DXVECTOR3(SelectBlockPos.x, SelectBlockPos.y, SelectBlockPos.z));
            }
        }
    }

    // 左コントロールとSキー画押されたとき
    if(m_pInputKeyboard->GetKeyboardPress(DIK_S) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_S)) {

        // コントロールとSキーが押されたことを判定
        m_bPushedControlS = true;
        // 保存出来たかのフラグを失敗の状態で初期化する
        m_bSaveJudge = false;
        //ブロックの座標を保存
        SaveStage();
    }

    // 左コントロールとVキーが押されたとき
    if(m_pInputKeyboard->GetKeyboardPress(DIK_V) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_V)) {

        //オブジェクトの情報が入っている場合
        if(m_nCopyType >= 0) {
            if(m_bGreaseJudge == false) {
                CreatBlockControl(m_FixedSshaft,                                   // グリッドを適用時の座標
                                  D3DXVECTOR3(m_posR.x, m_CopyPos.y, m_posR.z),    // カーソルの座標
                                  m_CopyRot,                                       // コピーしたオブジェクトの向き
                                  m_nCopyType,                                     // コピーしたオブジェクトの種類
                                  false,                                           // 変更を適用し逓倍状態にする
                                  nCount,                                          // 変更番号
                                  CScene::OBJTYPE_BLOCK,                           // オブジェクトのタイプ
                                  DETAILSCHANGE_CREATE);                           // 変更内容を保存
            }
            if(m_bGreaseJudge == true) {
                CreatBlockControl(m_FixedSshaft,                                             // グリッジド適用時の座標
                                  D3DXVECTOR3(m_CursorPos.x, m_CopyPos.y, m_CursorPos.z),    // カーソルの座標
                                  m_CopyRot,                                                 // コピーしたオブジェクトの向き
                                  m_nCopyType,                                               // コピーしたオブジェクトの種類
                                  false,                                                     // 変更を適用し逓倍状態にする
                                  nCount,                                                    // 変更番号
                                  CScene::OBJTYPE_BLOCK,                                     // オブジェクトのタイプ
                                  DETAILSCHANGE_CREATE);                                     // 変更内容を保存
            }
        }
    }

    //右クリックされた時
    if(m_pMouse->GetMouseTriggerRight()) {
        if(m_bGreaseJudge == false) {
            CreatBlockControl(m_FixedSshaft, D3DXVECTOR3(m_posR.x, 0, m_posR.z), D3DXVECTOR3(0, 0, 0), m_nBlockType, false, nCount, CScene::OBJTYPE_BLOCK, DETAILSCHANGE_CREATE);
        }
        if(m_bGreaseJudge == true) {
            CreatBlockControl(m_FixedSshaft, D3DXVECTOR3(m_CursorPos.x, 0, m_CursorPos.z), D3DXVECTOR3(0, 0, 0), m_nBlockType, false, nCount, CScene::OBJTYPE_BLOCK, DETAILSCHANGE_CREATE);
        }
    }
    //左クリックされた時
    if(m_pMouse->GetMouseTriggerLeft()) {
        //オブジェクトを選択してる場合
        if(CBlock::GetSelectBlock() != NULL) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // 消すオブジェクト情報を保存
            SaveDate(CBlock::GetSelectBlock()->GetPos(),
                     CBlock::GetSelectBlock()->GetRot(),
                     CBlock::GetSelectBlock()->GetNumber(),
                     DETAILSCHANGE_BREAK,
                     false,
                     CBlock::GetSelectBlock()->GetType(),
                     nCount,
                     CScene::OBJTYPE_BLOCK);
            //選択してるオブジェトを消去する
            CBlock::GetSelectBlock()->Uninit();
        }
    }

    // オブジェクト全消去を管理する変数が有効の場合
    if(m_bUseAllDelete == true) {
        // 設置されているオブジェクトを全て消去する
        nCount = BlockAllDelete(nCount);

        // オブジェクト総数が0になった場合、無効にする
        if(CBlock::GetBlockNumAll() <= 0) {
            m_bUseAllDelete = false;
        }
    }

    return nCount;
}

//=============================================================================
// 敵を設置
//=============================================================================
int CCreateStage::SetEnemy(CInputKeyboard *m_pInputKeyboard, D3DXVECTOR3 m_posR, CMouse *m_pMouse, int nCount, BLOCKSAVEDATE m_aBlockSaveDate, CREATEOBJECT m_CreateType)
{
    static D3DXVECTOR3 Pos;
    static D3DXVECTOR3 Rot;
    static int nNumber = -1;

    // タイプを管理する変数が範囲外にならないように処理
    if(m_nEnemyType >= CEnemy::TYPE_MAX) {
        m_nEnemyType = CEnemy::TYPE_MAX - 1;
    }
    if(m_nEnemyType < 0) {
        m_nEnemyType = 0;
    }

    //=================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================
    // 設置オブジェクトの変更

	// 左コントロールが押されていない場合
	if (!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
		// メイン数字キー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_1)) {
			SetObjectType(CEnemy::TYPE_1);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_2)) {
			SetObjectType(CEnemy::TYPE_2);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_3)) {
			SetObjectType(CEnemy::TYPE_3);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_4)) {
			SetObjectType(CEnemy::TYPE_4);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_5)) {
			SetObjectType(CEnemy::TYPE_5);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_6)) {
			SetObjectType(CEnemy::TYPE_6);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_7)) {
			SetObjectType(CEnemy::TYPE_7);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_8)) {
			SetObjectType(CEnemy::TYPE_8);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_9)) {
			SetObjectType(CEnemy::TYPE_9);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_0)) {
			SetObjectType(CEnemy::TYPE_10);
		}
		// テンキー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD1)) {
			SetObjectType(CEnemy::TYPE_11);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD2)) {
			SetObjectType(CEnemy::TYPE_12);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD3)) {
			SetObjectType(CEnemy::TYPE_13);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD4)) {
			SetObjectType(CEnemy::TYPE_14);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD5)) {
			SetObjectType(CEnemy::TYPE_15);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD6)) {
			SetObjectType(CEnemy::TYPE_16);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD7)) {
			SetObjectType(CEnemy::TYPE_17);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD8)) {
			SetObjectType(CEnemy::TYPE_18);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD9)) {
			SetObjectType(CEnemy::TYPE_19);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0)) {
			SetObjectType(CEnemy::TYPE_20);
		}
	}
	// 左コントロールが押されている場合
	else if (m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
		// メイン数字キー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_1)) {
			SetObjectType(CEnemy::TYPE_21);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_2)) {
			SetObjectType(CEnemy::TYPE_22);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_3)) {
			SetObjectType(CEnemy::TYPE_23);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_4)) {
			SetObjectType(CEnemy::TYPE_24);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_5)) {
			SetObjectType(CEnemy::TYPE_25);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_6)) {
			SetObjectType(CEnemy::TYPE_26);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_7)) {
			SetObjectType(CEnemy::TYPE_27);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_8)) {
			SetObjectType(CEnemy::TYPE_28);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_9)) {
			SetObjectType(CEnemy::TYPE_29);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_0)) {
			SetObjectType(CEnemy::TYPE_30);
		}
		// テンキー
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD1)) {
			SetObjectType(CEnemy::TYPE_31);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD2)) {
			SetObjectType(CEnemy::TYPE_32);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD3)) {
			SetObjectType(CEnemy::TYPE_33);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD4)) {
			SetObjectType(CEnemy::TYPE_34);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD5)) {
			SetObjectType(CEnemy::TYPE_35);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD6)) {
			SetObjectType(CEnemy::TYPE_36);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD7)) {
			SetObjectType(CEnemy::TYPE_37);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD8)) {
			SetObjectType(CEnemy::TYPE_38);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD9)) {
			SetObjectType(CEnemy::TYPE_39);
		}
		if (m_pInputKeyboard->GetKeyboardTrigger(DIK_NUMPAD0)) {
			SetObjectType(CEnemy::TYPE_40);
		}
	}
    //=================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================

    // 左コントロールとXキーが押されたとき選択中のオブジェクト情報を保存し、選択中のオブジェクトを消す
    if(m_pInputKeyboard->GetKeyboardPress(DIK_X) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_X)) {

        // オブジェクトを選択している場合
        if(CEnemy::GetSelectEnemy() != NULL) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // 消すオブジェクト情報を保存
            SaveDate(CEnemy::GetSelectEnemy()->GetPos(),
                     CEnemy::GetSelectEnemy()->GetRot(),
                     CEnemy::GetSelectEnemy()->GetNumber(),
                     DETAILSCHANGE_BREAK,
                     false,
                     CEnemy::GetSelectEnemy()->GetType(),
                     nCount,
                     CScene::OBJTYPE_ENEMY);
            // 選択中のオブジェクト情報を保存
            SetCopyDate(CEnemy::GetSelectEnemy()->GetPos(),
                        CEnemy::GetSelectEnemy()->GetRot(),
                        CEnemy::GetSelectEnemy()->GetType());
            // 選択中のオブジェクトを消す
            CEnemy::GetSelectEnemy()->Uninit();
        }
    }

    // コントロールとPキーが押された場合
    if(m_pInputKeyboard->GetKeyboardPress(DIK_P) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_P)) {

        if(CEnemy::GetEnemyNumAll() >= OBJECT_MIN_NUM) {
            // 設置されているオブジェクトを全て消去する関数を読み込む状態に変更
            m_bUseAllDelete = true;
            // 全消去実行直前のオブジェクトの総数を取得
            m_nAllDeleteNum = CEnemy::GetEnemyNumAll();
        }
    }

    // オブジェクトを選択出来ている場合
    if(CEnemy::GetSelectEnemy() != NULL) {
        // 変更前の情報を取得
        if(nNumber == -1 || nNumber != CEnemy::GetSelectEnemy()->GetNumber()) {
            Pos     = CEnemy::GetSelectEnemy()->GetPos();
            Rot     = CEnemy::GetSelectEnemy()->GetRot();
            nNumber = CEnemy::GetSelectEnemy()->GetNumber();
        }
        // 選択中のオブジェクト情報を取得
        D3DXVECTOR3 SelectBlockPos = CEnemy::GetSelectEnemy()->GetPos();
        D3DXVECTOR3 SelectBlockRot = CEnemy::GetSelectEnemy()->GetRot();

        // Q/Eキーで角度を90度毎に変化させる
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_Q)) {
            nAddupAngle += 90;
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_E)) {
            nAddupAngle -= 90;
        }

        // 左コントロールが押されていない場合
        if(!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            // U/Jキーで高差を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_U)) {
                SelectBlockPos.y += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_J)) {
                SelectBlockPos.y -= 15;
            }
            // I/KキーでX軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_I)) {
                SelectBlockPos.x += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_K)) {
                SelectBlockPos.x -= 15;
            }
            // O/Lキー軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_O)) {
                SelectBlockPos.z += 15;
            }
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_L)) {
                SelectBlockPos.z -= 15;
            }
        }
        // 左コントロールが押されている場合
        else if(m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            // U/Jキーで高差を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_U)) {
                SelectBlockPos.y += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_J)) {
                SelectBlockPos.y -= 5;
            }
            // I/KキーでX軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_I)) {
                SelectBlockPos.x += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_K)) {
                SelectBlockPos.x -= 5;
            }
            // O/Lキー軸を15ずつ変化させる
            if(m_pInputKeyboard->GetKeyboardPress(DIK_O)) {
                SelectBlockPos.z += 5;
            }
            if(m_pInputKeyboard->GetKeyboardPress(DIK_L)) {
                SelectBlockPos.z -= 5;
            }
        }

        // F1/F2キーで選択中オブジェクトの情報を設置された状態に戻す
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_F1)) {
            CEnemy::GetSelectEnemy()->SetPos(D3DXVECTOR3(Pos.x, Pos.y, Pos.z));
        }
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_F2)) {
            CEnemy::GetSelectEnemy()->SetRot(D3DXVECTOR3(Rot.x, Rot.y, Rot.z));
        }

        // 左コントロールとCキーが押された場合
        if(m_pInputKeyboard->GetKeyboardPress(DIK_C) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_C)) {
			// コピーするタイプを変数に保存する
			m_nCopyType = CBlock::GetSelectBlock()->GetType();

            // 選択中のオブジェクト情報を保存する
            SetCopyDate(SelectBlockPos, SelectBlockRot, m_nCopyType);
        }

        //　加算/減算させる変数が大きすぎないように調整
        if(nAddupAngle % 360 == 0) {
            nAddupAngle = 0;
        }
        if(SelectBlockPos.y > 10000) {
            SelectBlockPos.y = 10000;
        }
        if(SelectBlockPos.y < -10000) {
            SelectBlockPos.y = -10000;
        }

        // Q/Eキーが押された場合
        if(m_pInputKeyboard->GetKeyboardTrigger(DIK_Q) || m_pInputKeyboard->GetKeyboardTrigger(DIK_E)) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // オブジェクトの向きを1桁目切り捨てする
            int RotX = (int)CEnemy::GetSelectEnemy()->GetRot().x / 10 * 10;
            int RotY = (int)CEnemy::GetSelectEnemy()->GetRot().y / 10 * 10;
            int RotZ = (int)CEnemy::GetSelectEnemy()->GetRot().z / 10 * 10;
            // 変更情報を保存する
            SaveDate(CEnemy::GetSelectEnemy()->GetPos(),
                     CEnemy::GetSelectEnemy()->GetRot(),
                     CEnemy::GetSelectEnemy()->GetNumber(),
                     DETAILSCHANGE_CHANGE_ROT,
                     false,
                     CEnemy::GetSelectEnemy()->GetType(),
                     nCount,
                     CEnemy::OBJTYPE_ENEMY);

            // 選択中のオブジェクトの向きを変更
            CEnemy::GetSelectEnemy()->SetRot(
              D3DXVECTOR3(D3DXToRadian(RotX), D3DXToRadian(RotY + nAddupAngle), D3DXToRadian(RotZ)));
        }
        // 左コントロールが押されていない場合
        if(!m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            if(m_pInputKeyboard->GetKeyboardTrigger(DIK_U) || m_pInputKeyboard->GetKeyboardTrigger(DIK_J) || m_pInputKeyboard->GetKeyboardTrigger(DIK_I) || m_pInputKeyboard->GetKeyboardTrigger(DIK_K) || m_pInputKeyboard->GetKeyboardTrigger(DIK_O) || m_pInputKeyboard->GetKeyboardTrigger(DIK_L)) {
                // 配列の確認
                nCount = ShiftArray(nCount);
                // 変更情報を保存する
                SaveDate(CEnemy::GetSelectEnemy()->GetPos(),
                         CEnemy::GetSelectEnemy()->GetRot(),
                         CEnemy::GetSelectEnemy()->GetNumber(),
                         DETAILSCHANGE_CHANGE_POS,
                         false,
                         CEnemy::GetSelectEnemy()->GetType(),
                         nCount,
                         CEnemy::OBJTYPE_ENEMY);

                // 選択中のオブジェクトの座標を変更
                CEnemy::GetSelectEnemy()->SetPos(
                  D3DXVECTOR3(SelectBlockPos.x, SelectBlockPos.y, SelectBlockPos.z));
            }
        }
        // 左コントロールが押されている場合
        else if(m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL)) {
            if(m_pInputKeyboard->GetKeyboardPress(DIK_U) || m_pInputKeyboard->GetKeyboardPress(DIK_J) || m_pInputKeyboard->GetKeyboardPress(DIK_I) || m_pInputKeyboard->GetKeyboardPress(DIK_K) || m_pInputKeyboard->GetKeyboardPress(DIK_O) || m_pInputKeyboard->GetKeyboardPress(DIK_L)) {
                // 配列の確認
                nCount = ShiftArray(nCount);
                // 変更情報を保存する
                SaveDate(CEnemy::GetSelectEnemy()->GetPos(),
                         CEnemy::GetSelectEnemy()->GetRot(),
                         CEnemy::GetSelectEnemy()->GetNumber(),
                         DETAILSCHANGE_CHANGE_POS,
                         false,
                         CEnemy::GetSelectEnemy()->GetType(),
                         nCount,
                         CEnemy::OBJTYPE_ENEMY);

                // 選択中のオブジェクトの座標を変更
                CEnemy::GetSelectEnemy()->SetPos(
                  D3DXVECTOR3(SelectBlockPos.x, SelectBlockPos.y, SelectBlockPos.z));
            }
        }
    }

    // 左コントロールとSキー画押されたとき
    if(m_pInputKeyboard->GetKeyboardPress(DIK_S) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_S)) {

        // コントロールとSキーが押されたことを判定
        m_bPushedControlS = true;
        // 保存出来たかのフラグを失敗の状態で初期化する
        m_bSaveJudge = false;
        //ブロックの座標を保存
        SaveEnemy();
    }

    // 左コントロールとVキーが押されたとき
    if(m_pInputKeyboard->GetKeyboardPress(DIK_V) && m_pInputKeyboard->GetKeyboardTrigger(DIK_LCONTROL) || m_pInputKeyboard->GetKeyboardPress(DIK_LCONTROL) && m_pInputKeyboard->GetKeyboardTrigger(DIK_V)) {

        //オブジェクトの情報が入っている場合
        if(m_nCopyType >= 0) {
            if(m_bGreaseJudge = false) {
                CreatBlockControl(m_FixedSshaft,                                   // グリッドを適用時の座標
                                  D3DXVECTOR3(m_posR.x, m_CopyPos.y, m_posR.z),    // カーソルの座標
                                  m_CopyRot,                                       // コピーしたオブジェクトの向き
                                  m_nCopyType,                                     // コピーしたオブジェクトの種類
                                  false,                                           // 変更を適用し逓倍状態にする
                                  nCount,                                          // 変更番号
                                  CScene::OBJTYPE_ENEMY,                           // オブジェクトのタイプ
                                  DETAILSCHANGE_CREATE);                           // 変更内容を保存
            }
            if(m_bGreaseJudge = true) {
                CreatBlockControl(m_FixedSshaft,                                             // グリッジド適用時の座標
                                  D3DXVECTOR3(m_CursorPos.x, m_CopyPos.y, m_CursorPos.z),    // カーソルの座標
                                  m_CopyRot,                                                 // コピーしたオブジェクトの向き
                                  m_nCopyType,                                               // コピーしたオブジェクトの種類
                                  false,                                                     // 変更を適用し逓倍状態にする
                                  nCount,                                                    // 変更番号
                                  CScene::OBJTYPE_ENEMY,                                     // オブジェクトのタイプ
                                  DETAILSCHANGE_CREATE);                                     // 変更内容を保存
            }
        }
    }

    //右クリックされた時
    if(m_pMouse->GetMouseTriggerRight()) {
		if (m_bGreaseJudge == false) {
			CreatBlockControl(m_FixedSshaft, D3DXVECTOR3(m_posR.x, 0, m_posR.z), D3DXVECTOR3(0, 0, 0), m_nEnemyType, false, nCount, CScene::OBJTYPE_ENEMY, DETAILSCHANGE_CREATE);
		}
		if (m_bGreaseJudge == true) {
			CreatBlockControl(m_FixedSshaft, D3DXVECTOR3(m_CursorPos.x, 0, m_CursorPos.z), D3DXVECTOR3(0, 0, 0), m_nEnemyType, false, nCount, CScene::OBJTYPE_ENEMY, DETAILSCHANGE_CREATE);
		}
    }
    //左クリックされた時
    if(m_pMouse->GetMouseTriggerLeft()) {
        //オブジェクトを選択してる場合
        if(CEnemy::GetSelectEnemy() != NULL) {
            // 配列の確認
            nCount = ShiftArray(nCount);
            // 消すオブジェクト情報を保存
            SaveDate(CEnemy::GetSelectEnemy()->GetPos(),
                     CEnemy::GetSelectEnemy()->GetRot(),
                     CEnemy::GetSelectEnemy()->GetNumber(),
                     DETAILSCHANGE_BREAK,
                     false,
                     CEnemy::GetSelectEnemy()->GetType(),
                     nCount,
                     CScene::OBJTYPE_ENEMY);
            //選択してるオブジェトを消去する
            CEnemy::GetSelectEnemy()->Uninit();
        }
    }

    // オブジェクト全消去を管理する変数が有効の場合
    if(m_bUseAllDelete == true) {
        // 設置されているオブジェクトを全て消去する
        nCount = EnemyAllDelete(nCount);

        // 敵の総数が0の場合、無効にする
        if(CEnemy::GetEnemyNumAll() <= 0) {
            m_bUseAllDelete = false;
        }
    }

    return nCount;
}

//=============================================================================
// 設置されているオブジェクトを全て消去する
//=============================================================================
int CCreateStage::BlockAllDelete(int nCount)
{
    // ブロックとの当たり判定
    CScene *pScene = CScene::GetTopScene();

    for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
        // 中身があるなら
        if(pScene != NULL) {
            // 次のシーンを記憶
            CScene *pNextScene = pScene->GetNextScene();

            // タイプを取得
            CScene::OBJTYPE objType = pScene->GetObjType();

            // ブロックなら、
            if(objType == CScene::OBJTYPE_BLOCK) {
                // ブロックにキャスト
                CBlock *pBlock = (CBlock *)pScene;

                // 配列の確認
                nCount = ShiftArray(nCount);

                // 消すオブジェクト情報を保存
                SaveDate(pBlock->GetPos(),
                         pBlock->GetRot(),
                         pBlock->GetNumber(),
                         DETAILSCHANGE_TOTALELIMINATION,
                         false,
                         pBlock->GetType(),
                         nCount,
                         CScene::OBJTYPE_BLOCK);

                pBlock->Uninit();

                nCount++;
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    return nCount;
}

//=============================================================================
// 設置されている敵を全て消去する
//=============================================================================
int CCreateStage::EnemyAllDelete(int nCount)
{
    // ブロックとの当たり判定
    CScene *pScene = CScene::GetTopScene();

    for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
        // 中身があるなら
        if(pScene != NULL) {
            // 次のシーンを記憶
            CScene *pNextScene = pScene->GetNextScene();

            // タイプを取得
            CScene::OBJTYPE objType = pScene->GetObjType();

            // 敵なら、
            if(objType == CScene::OBJTYPE_ENEMY) {
                // 敵にキャスト
                CEnemy *pEnemy = (CEnemy *)pScene;

                // 配列の確認
                nCount = ShiftArray(nCount);

                // 消すオブジェクト情報を保存
                SaveDate(pEnemy->GetPos(),
                         pEnemy->GetRot(),
                         pEnemy->GetNumber(),
                         DETAILSCHANGE_TOTALELIMINATION,
                         false,
                         pEnemy->GetType(),
                         nCount,
                         CScene::OBJTYPE_ENEMY);

                pEnemy->Uninit();

                nCount++;
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    return nCount;
}

//=============================================================================
// ブロック設置の制御
//=============================================================================
void CCreateStage::CreatBlockControl(D3DXVECTOR3 FixedSshaft,
                                     D3DXVECTOR3 CursorPos,
                                     D3DXVECTOR3 PosR,
                                     int nType,
                                     bool Judge,
                                     int nCount,
                                     CScene::OBJTYPE ObjectType,
                                     DETAILSCHANGE DetallsChange)
{
	// 情報を一時保存する用変数
    D3DXVECTOR3 pos;
    int nNumber;

    // オブジェクトの種類によって生成するクラスを切り替える
    switch(ObjectType) {
        case CScene::OBJTYPE_BLOCK:

            if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == false) {
                // オブジェクトを設置
                CBlock::Create(D3DXVECTOR3(CursorPos.x, CursorPos.y, CursorPos.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
                pos = CursorPos;
            }
            else if(m_bFixedSshaft[0] == true && m_bFixedSshaft[1] == false) {
                // オブジェクトを設置
                CBlock::Create(D3DXVECTOR3(FixedSshaft.x, CursorPos.y, CursorPos.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
                pos = D3DXVECTOR3(FixedSshaft.x, CursorPos.y, CursorPos.z);
            }
            else if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == true) {
                // オブジェクトを設置
                CBlock::Create(D3DXVECTOR3(CursorPos.x, CursorPos.y, FixedSshaft.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
				pos = D3DXVECTOR3(CursorPos.x, CursorPos.y, FixedSshaft.z);
            }
            else {
                // オブジェクトを設置
                CBlock::Create(D3DXVECTOR3(FixedSshaft.x, CursorPos.y, FixedSshaft.z),
                               PosR,
                               nType);

                // 設置するオブジェクトの情報を保存
                pos = D3DXVECTOR3(FixedSshaft.x, CursorPos.y, FixedSshaft.z);
            }

            nNumber = (CBlock::GetBlockNumAll() - NEXT_DATA);

            break;

        case CScene::OBJTYPE_ENEMY:

            if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == false) {
                // 敵を設置
                CEnemy::Create(D3DXVECTOR3(CursorPos.x, CursorPos.y, CursorPos.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
                pos = CursorPos;
            }
            else if(m_bFixedSshaft[0] == true && m_bFixedSshaft[1] == false) {
                // 敵を設置
                CEnemy::Create(D3DXVECTOR3(FixedSshaft.x, CursorPos.y, CursorPos.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
                pos = D3DXVECTOR3(FixedSshaft.x, CursorPos.y, CursorPos.z);
            }
            else if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == true) {
                // 敵を設置
                CEnemy::Create(D3DXVECTOR3(CursorPos.x, CursorPos.y, FixedSshaft.z), PosR, nType);

                // 設置するオブジェクトの情報を保存
                pos = D3DXVECTOR3(CursorPos.x, CursorPos.y, FixedSshaft.z);
            }
            else {
                // 敵を設置
                CEnemy::Create(D3DXVECTOR3(FixedSshaft.x, CursorPos.y, FixedSshaft.z),
                               PosR,
                               nType);

                // 設置するオブジェクトの情報を保存
                pos = D3DXVECTOR3(FixedSshaft.x, CursorPos.y, FixedSshaft.z);
            }

            nNumber = (CEnemy::GetEnemyNumAll() - NEXT_DATA);

            break;
    }

    // 設置するオブジェクト情報を保存
    SaveDate(pos,
             PosR,
             nNumber,
             DetallsChange,
             false,
             nType,
             nCount,
             ObjectType);
}

//=============================================================================
// カーソルの制御
//=============================================================================
void CCreateStage::CursorControl(D3DXVECTOR3 FixedSshaft, D3DXVECTOR3 CursorPos, CREATEOBJECT m_CreateType)
{
    if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == false) {
        // 置く場所を分かりやすく
        CEffect3D::Create(D3DXVECTOR3(CursorPos.x, CURSOR_POS_Y, CursorPos.z),
                          CURSOR_SIZE,
                          DEFAULT_VECTOR,
                          DEFAULT_COLOR,
                          CEffect3D::TYPE_CURSOR);
        // カーソルの当たり判定
        CursorCollision(D3DXVECTOR3(CursorPos.x, CURSOR_POS_Y, CursorPos.z),
                        m_CreateType);
    }
    else if(m_bFixedSshaft[0] == true && m_bFixedSshaft[1] == false) {
        // 置く場所を分かりやすく
        CEffect3D::Create(D3DXVECTOR3(FixedSshaft.x, CURSOR_POS_Y, CursorPos.z),
                          CURSOR_SIZE,
                          DEFAULT_VECTOR,
                          DEFAULT_COLOR,
                          CEffect3D::TYPE_CURSOR);
        // カーソルの当たり判定
        CursorCollision(D3DXVECTOR3(FixedSshaft.x, CURSOR_POS_Y, CursorPos.z),
                        m_CreateType);
    }
    else if(m_bFixedSshaft[0] == false && m_bFixedSshaft[1] == true) {
        // 置く場所を分かりやすく
        CEffect3D::Create(D3DXVECTOR3(CursorPos.x, CURSOR_POS_Y, FixedSshaft.z),
                          CURSOR_SIZE,
                          DEFAULT_VECTOR,
                          DEFAULT_COLOR,
                          CEffect3D::TYPE_CURSOR);
        // カーソルの当たり判定
        CursorCollision(D3DXVECTOR3(CursorPos.x, CURSOR_POS_Y, FixedSshaft.z),
                        m_CreateType);
    }
    else {
        // 置く場所を分かりやすく
        CEffect3D::Create(D3DXVECTOR3(FixedSshaft.x, CURSOR_POS_Y, FixedSshaft.z),
                          CURSOR_SIZE,
                          DEFAULT_VECTOR,
                          DEFAULT_COLOR,
                          CEffect3D::TYPE_CURSOR);
        // カーソルの当たり判定
        CursorCollision(D3DXVECTOR3(FixedSshaft.x, CURSOR_POS_Y, FixedSshaft.z),
                        m_CreateType);
    }
}

//=============================================================================
// カーソルの当たり判定
//=============================================================================
void CCreateStage::CursorCollision(D3DXVECTOR3 pos, CREATEOBJECT m_CreateType)
{
    // ブロックとの当たり判定
    CScene *pScene    = CScene::GetTopScene();
    bool bSelectBlock = false;    // 選択しているかどうかのフラグ
    bool bSelectEnemy = false;
    for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
        // 中身があるなら
        if(pScene != NULL) {
            // 次のシーンを記憶
            CScene *pNextScene = pScene->GetNextScene();

            // タイプを取得
            CScene::OBJTYPE objType = pScene->GetObjType();

            // ブロックなら、
            if(objType == CScene::OBJTYPE_BLOCK && m_CreateType == CREATEOBJECT_STAGE) {
                // ブロックにキャスト
                CBlock *pBlock = (CBlock *)pScene;

                // カーソルの当たり判定のサイズを設定
                D3DXVECTOR3 cursorCollisionSize = CURSOR_COLLISION_SIZE;

                // カーソルの当たり判定を計算
                if(RectangleCollision3D(&pos, &pBlock->GetPos(), &cursorCollisionSize, &pBlock->GetCollisionSize()) == true) {
                    // 当たっているなら、選択しているブロックにする
                    CBlock::SetSelectBlock(pBlock);

                    // 選択している
                    bSelectBlock = true;
                }
            }

            // 敵なら、
            if(objType == CScene::OBJTYPE_ENEMY && m_CreateType == CREATEOBJECT_ENEMY) {
                // 敵にキャスト
                CEnemy *pEnemy = (CEnemy *)pScene;

                // カーソルの当たり判定のサイズを設定
                D3DXVECTOR3 cursorCollisionSize = CURSOR_COLLISION_SIZE;

                // カーソルの当たり判定を計算
                if(RectangleCollision3D(&pos, &pEnemy->GetPos(), &cursorCollisionSize, &pEnemy->GetCollisionSize()) == true) {
                    // 当たっているなら、選択しているブロックにする
                    CEnemy::SetSelectEnemy(pEnemy);

                    // 選択している
                    bSelectEnemy = true;
                }
            }

            // 次のシーンにする
            pScene = pNextScene;
        }
        else {
            // 中身がないなら、そこで処理を終える
            break;
        }
    }

    // 選択しているものがないなら
    if(bSelectBlock == false) {
        // 選択しているブロックをNULLにする
        CBlock::SetSelectBlock(NULL);
    }
    // 選択しているものが無い場合
    if(bSelectEnemy == false) {
        // 選択している敵をNULLにする
        CEnemy::SetSelectEnemy(NULL);
    }
}

//=============================================================================
// ステージのオブジェクトを読み込む
//=============================================================================
HRESULT CCreateStage::LoadStage(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int nCntLoad = 0;
    char cReedText[128];    // 文字として読み取り用
    char cHeadText[256];    //
    char cDie[128];

    // デバッグ用の変数
    int nType[256];
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // ファイル開
    pFile = fopen(m_cTxtName[0], "r");

    CField::SetMapSiz(D3DXVECTOR3(MAP_WIDTH, MAP_HEIGHT, 0));

    // 開けた
    if(pFile != NULL) {
        while(strcmp(cHeadText, "MODELSET") != 0) {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);

            if(strcmp(cHeadText, "MAP_SIZ") == 0) {
                sscanf(cReedText, "%s %s %f %f", &cDie, &cDie, &m_FieldSiz.x, &m_FieldSiz.y);
                CField::SetMapSiz(m_FieldSiz);
            }
        }

        if(strcmp(cHeadText, "MODELSET") == 0) {
            while(strcmp(cHeadText, "END_SCRIPT") != 0) {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if(strcmp(cHeadText, "\n") == 0) {
                }
                else if(strcmp(cHeadText, "END_MODELSET") != 0) {
                    if(strcmp(cHeadText, "TYPE") == 0) {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if(strcmp(cHeadText, "POS") == 0) {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if(strcmp(cHeadText, "ROT") == 0) {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        // ブロックを生成
                        CBlock::Create(pos[nCntLoad],
                                       D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                                   D3DXToRadian(rot[nCntLoad].y),
                                                   D3DXToRadian(rot[nCntLoad].z)),
                                       nType[nCntLoad]);

                        nCntLoad++;
                    }
                }
            }
        }
        // ファイル閉
        fclose(pFile);

        return S_OK;
    }

    // 開けない
    else {
        printf("開けれませんでした\n");
    }

    return E_FAIL;
}

//=============================================================================
// ステージの敵を読み込む
//=============================================================================
HRESULT CCreateStage::LoadEnemy(void)
{
    // ファイルポイント
    FILE *pFile = NULL;

    // 変数宣言
    int nCntLoad = 0;
    char cReedText[128];    // 文字として読み取り用
    char cHeadText[256];    //
    char cDie[128];

    // デバッグ用の変数
    int nType[256];
    D3DXVECTOR3 pos[256];
    D3DXVECTOR3 rot[256];

    // ファイル開
    pFile = fopen(m_cTxtName[1], "r");

    // 開けた
    if(pFile != NULL) {
        while(strcmp(cHeadText, "ENEMYSET") != 0) {
            fgets(cReedText, sizeof(cReedText), pFile);
            sscanf(cReedText, "%s", &cHeadText);
        }

        if(strcmp(cHeadText, "ENEMYSET") == 0) {
            while(strcmp(cHeadText, "END_SCRIPT") != 0) {
                fgets(cReedText, sizeof(cReedText), pFile);
                sscanf(cReedText, "%s", &cHeadText);

                if(strcmp(cHeadText, "\n") == 0) {
                }
                else if(strcmp(cHeadText, "END_ENEMYSET") != 0) {
                    if(strcmp(cHeadText, "TYPE") == 0) {
                        sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nType[nCntLoad]);
                    }

                    if(strcmp(cHeadText, "POS") == 0) {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos[nCntLoad].x, &pos[nCntLoad].y, &pos[nCntLoad].z);
                    }

                    if(strcmp(cHeadText, "ROT") == 0) {
                        sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot[nCntLoad].x, &rot[nCntLoad].y, &rot[nCntLoad].z);

                        // ブロックを生成
                        CEnemy::Create(pos[nCntLoad],
                                       D3DXVECTOR3(D3DXToRadian(rot[nCntLoad].x),
                                                   D3DXToRadian(rot[nCntLoad].y),
                                                   D3DXToRadian(rot[nCntLoad].z)),
                                       nType[nCntLoad]);

                        nCntLoad++;
                    }
                }
            }
        }
        // ファイル閉
        fclose(pFile);

        return S_OK;
    }

    // 開けない
    else {
        printf("開けれませんでした\n");
    }

    return E_FAIL;
}

//=============================================================================
// ステージのオブジェクト情報を外部出力
//=============================================================================
HRESULT CCreateStage::SaveStage(void)
{
    FILE *fp;

    if(CBlock::GetBlockNumAll() != 0) {

        fp = fopen(m_cTxtName[0], "w");

        if(fp != NULL) {
            // オブジェクトの先頭の情報を取得
            CScene *pScene = CScene::GetTopScene();

            fprintf(fp, "SCRIPT			# "
                        "この行は絶対消さないこと！\n\n");

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 読み込むモデル数\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "NUM_MODEL = %d\n\n", CBlock::GetBlockNumAll());

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# マップサイズ\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            // マップのサイズを取得し、大きさが1マス以上存在する場合、現在の大きさを書き込む
            if(m_FieldSiz.x >= 1 && m_FieldSiz.y >= 1) {
                fprintf(fp, "MAP_SIZ = %d %d\n\n", (int)m_FieldSiz.x, (int)m_FieldSiz.y);
            }
            // マップのサイズが1マス未満の場合、マクロ定義されている大きさを書き込む
            else {
                fprintf(fp, "MAP_SIZ = %d %d\n\n", MAP_WIDTH, MAP_HEIGHT);
            }

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 読み込むモデルファイル名\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "\n\n");

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 表示するモデルの配置情報\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");

            for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
                // 中身があるなら
                if(pScene != NULL) {
                    // 次のシーンを記憶
                    CScene *pNextScene = pScene->GetNextScene();

                    // タイプを取得
                    CScene::OBJTYPE objType = pScene->GetObjType();

                    if(objType == CScene::OBJTYPE_BLOCK) {
                        // ブロックにキャスト
                        CBlock *pBlock = (CBlock *)pScene;

                        fprintf(fp, "MODELSET\n");

                        fprintf(fp, "	TYPE = %d\n", pBlock->GetType());
                        fprintf(fp, "	POS = %d %d %d\n", (int)pBlock->GetPos().x, (int)pBlock->GetPos().y, (int)pBlock->GetPos().z);
                        fprintf(fp, "	ROT = %d %d %d\n", (int)D3DXToDegree(pBlock->GetRot().x), (int)D3DXToDegree(pBlock->GetRot().y), (int)D3DXToDegree(pBlock->GetRot().z));

                        fprintf(fp, "END_MODELSET\n\n");
                    }

                    // 次のシーンにする
                    pScene = pNextScene;
                }
                else {
                    // 中身がないなら、そこで処理を終える
                    break;
                }
            }

            fprintf(fp, "END_SCRIPT		# この行は絶対消さないこと！");

            fclose(fp);

            m_bSaveJudge = true;

            return S_OK;
        }
    }

    return E_FAIL;
}

//=============================================================================
// ステージの敵情報を外部出力
//=============================================================================
HRESULT CCreateStage::SaveEnemy(void)
{
    FILE *fp;

    if(CEnemy::GetEnemyNumAll() != 0) {

        fp = fopen(m_cTxtName[1], "w");

        if(fp != NULL) {
            // オブジェクトの先頭の情報を取得
            CScene *pScene = CScene::GetTopScene();

            fprintf(fp, "SCRIPT			# "
                        "この行は絶対消さないこと！\n\n");

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 読み込むモデル数\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "NUM_MODEL = %d\n\n", CEnemy::GetEnemyNumAll());

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 読み込むモデルファイル名\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "\n\n");

            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");
            fprintf(fp, "# 表示するモデルの配置情報\n");
            fprintf(fp, "#-----------------------------------------------------------"
                        "-------------------\n");

            for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
                // 中身があるなら
                if(pScene != NULL) {
                    // 次のシーンを記憶
                    CScene *pNextScene = pScene->GetNextScene();

                    // タイプを取得
                    CScene::OBJTYPE objType = pScene->GetObjType();

                    if(objType == CScene::OBJTYPE_ENEMY) {
                        // ブロックにキャスト
                        CEnemy *pEnemy = (CEnemy *)pScene;

                        fprintf(fp, "ENEMYSET\n");

                        fprintf(fp, "	TYPE = %d\n", pEnemy->GetType());
                        fprintf(fp, "	POS = %d %d %d\n", (int)pEnemy->GetPos().x, (int)pEnemy->GetPos().y, (int)pEnemy->GetPos().z);
                        fprintf(fp, "	ROT = %d %d %d\n", (int)D3DXToDegree(pEnemy->GetRot().x), (int)D3DXToDegree(pEnemy->GetRot().y), (int)D3DXToDegree(pEnemy->GetRot().z));

                        fprintf(fp, "END_ENEMYSET\n\n");
                    }

                    // 次のシーンにする
                    pScene = pNextScene;
                }
                else {
                    // 中身がないなら、そこで処理を終える
                    break;
                }
            }

            fprintf(fp, "END_SCRIPT		# この行は絶対消さないこと！");

            fclose(fp);

            m_bSaveJudge = true;

            return S_OK;
        }
    }

    return E_FAIL;
}

//=============================================================================
// 変更内容を外部出力
//=============================================================================
void CCreateStage::WriteBlockSaveDate(int nNumAll)
{
    FILE *fp;
    fp = fopen(DEBUG_TXT_NAME, "w");

    if(fp != NULL) {
        fprintf(fp, "SCRIPT			# この行は絶対消さないこと！\n\n");

        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "# 保存数\n");
        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "SAVE_NUM = %d\n\n", nNumAll);

        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "# 変更内容と変更番号\n");
        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "%d:DETAILSCHANGE_CHANGE_POS\n", DETAILSCHANGE_CHANGE_POS);
        fprintf(fp, "%d:DETAILSCHANGE_CHANGE_LAST_POS\n", DETAILSCHANGE_CHANGE_LAST_POS);
        fprintf(fp, "%d:DETAILSCHANGE_CHANGE_ROT\n", DETAILSCHANGE_CHANGE_ROT);
        fprintf(fp, "%d:DETAILSCHANGE_CHANGE_LAST_ROT\n", DETAILSCHANGE_CHANGE_LAST_ROT);
        fprintf(fp, "%d:DETAILSCHANGE_BREAK\n", DETAILSCHANGE_BREAK);
        fprintf(fp, "%d:DETAILSCHANGE_CREATE\n", DETAILSCHANGE_CREATE);
        fprintf(fp, "%d:DETAILSCHANGE_TOTALELIMINATION\n", DETAILSCHANGE_TOTALELIMINATION);
        fprintf(fp, "\n");

        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "# オブジェクト番号と種類\n");
        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "%d:Block\n", CScene::OBJTYPE_BLOCK);
        fprintf(fp, "%d:Enemy\n", CScene::OBJTYPE_ENEMY);
        fprintf(fp, "\n\n");

        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");
        fprintf(fp, "# 変更前のオブジェクト情報\n");
        fprintf(fp, "#-------------------------------------------------------------"
                    "-----------------\n");

        for(int nCntScene = 0; nCntScene < nNumAll; nCntScene++) {
            if(m_aBlockSaveDate[nCntScene].DetailsChange != DETAILSCHANGE_NULL) {

                fprintf(fp, "DETAILSCHANGE : %d\n", nCntScene);

                fprintf(fp, "	NUMBER = %d\n", m_aBlockSaveDate[nCntScene].nNumber);
                fprintf(fp, "	DETAILSCHANGE = %d\n", m_aBlockSaveDate[nCntScene].DetailsChange);
                fprintf(fp, "	OBJTYPE = %d\n", m_aBlockSaveDate[nCntScene].ObjectType);
                fprintf(fp, "	TYPE = %d\n", m_aBlockSaveDate[nCntScene].nType);
                fprintf(fp, "	POS = %d %d %d\n", (int)m_aBlockSaveDate[nCntScene].pos.x, (int)m_aBlockSaveDate[nCntScene].pos.y, (int)m_aBlockSaveDate[nCntScene].pos.z);
                fprintf(fp, "	ROT = %d %d %d\n", (int)D3DXToDegree(m_aBlockSaveDate[nCntScene].rot.x), (int)D3DXToDegree(m_aBlockSaveDate[nCntScene].rot.y), (int)D3DXToDegree(m_aBlockSaveDate[nCntScene].rot.z));

                fprintf(fp, "END_DETAILSCHANGE\n\n");
            }
            else {
                break;
            }
        }

        fprintf(fp, "END_SCRIPT		# この行は絶対消さないこと！");

        fclose(fp);
    }
}

//=============================================================================
// 変更内容の情報を元に戻す
//=============================================================================
int CCreateStage::UndoingOperation(int nCurNum)
{
    // 「現在の配列の1つ後ろが存在している場合」かつ、「現在の配列の1つ後ろの変更が適用されていない状態の場合」かつ、「現在の配列の1つ後ろの変更内容が登録されている場合」
    if(nCurNum - NEXT_DATA >= 0 && m_aBlockSaveDate[nCurNum - NEXT_DATA].bChangeJudge == false && m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange != DETAILSCHANGE_NULL) {
        // 先頭のオブジェクト情報を取得
        CScene *pScene = CScene::GetTopScene();
        CBlock *pBlock;
        CEnemy *pEnemy;
        bool bSelectBlockJudge = false;    // 変更されたブロックオブジェクトを特定出来たかを判定
        bool bSelectEnemyJudge = false;    // 変更された敵オブジェクトを特定出来たかを判定
        bool bCheckData        = false;    // 特定したオブジェクトと変更内容の情報が全て一致しているかを判定
        // 「変更内容が消去以外の場合」かつ、「変更内容が全消去以外の場合」
        if(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange != DETAILSCHANGE_BREAK && m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange != DETAILSCHANGE_TOTALELIMINATION) {
            // 変更が適用されたオブジェクトを捜索する
            for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
                // 中身があるなら
                if(pScene != NULL) {
                    // 次のシーンを記憶
                    CScene *pNextScene = pScene->GetNextScene();

                    // タイプを取得
                    CScene::OBJTYPE objType = pScene->GetObjType();

                    // ブロックなら、
                    if(objType == CScene::OBJTYPE_BLOCK && m_aBlockSaveDate[nCurNum - NEXT_DATA].ObjectType == CScene::OBJTYPE_BLOCK) {
                        // ブロックにキャスト
                        pBlock = (CBlock *)pScene;

                        // 番号を取得
                        int nNumber = pBlock->GetNumber();
                        // 種類を取得
                        int nType = pBlock->GetType();

                        // ブロックにキャストしたオブジェクトと変更されたオブジェクトの番号が一致する場合
                        if(nNumber == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber && nType == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType) {
                            // 特定出来た状態に変更
                            bSelectBlockJudge = true;
                            break;
                        }
                    }

                    // 敵なら、
                    if(objType == CScene::OBJTYPE_ENEMY && m_aBlockSaveDate[nCurNum - NEXT_DATA].ObjectType == CScene::OBJTYPE_ENEMY) {
                        // 敵にキャスト
                        pEnemy = (CEnemy *)pScene;

                        // 番号を取得
                        int nNumber = pEnemy->GetNumber();
                        // 種類を取得
                        int nType = pEnemy->GetType();

                        // 敵にキャストしたオブジェクトと変更されたオブジェクトの番号が一致する場合
                        if(nNumber == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber && nType == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType) {
                            // 特定出来た状態に変更
                            bSelectEnemyJudge = true;
                            break;
                        }
                    }

                    // 次のシーンにする
                    pScene = pNextScene;
                }
                else {
                    // 中身がないなら、そこで処理を終える
                    break;
                }
            }
        }

		// オブジェクトタイプがブロックの場合
        if(m_aBlockSaveDate[nCurNum - NEXT_DATA].ObjectType == CScene::OBJTYPE_BLOCK) {
            // 「変更が適用されたブロックを特定出来た場合」又は、「変更内容が消去の場合」
            if(bSelectBlockJudge == true || m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_BREAK || m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                // 変更内容が消去の場合
                if(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_BREAK) {
                    CBlock::Create(m_aBlockSaveDate[nCurNum - NEXT_DATA].pos,
                                   m_aBlockSaveDate[nCurNum - NEXT_DATA].rot,
                                   m_aBlockSaveDate[nCurNum - NEXT_DATA].nType);
                }
                // 変更内容が全消去の場合
                else if(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                    // ブロックの順番と数を管理する変数
                    int nBlockCount = 0;
                    // ブロックの数が全消去した数以下の間無限にループさせる
                    while(nBlockCount <= m_nAllDeleteNum) {
                        // 全消去した数分配列を確認する
                        for(int nCount = 1; nCount <= m_nAllDeleteNum; nCount++) {
                            // オブジェクトを全消去した瞬間の番号順に生成
                            if(m_aBlockSaveDate[nCurNum - nCount].nNumber == nBlockCount) {
                                CBlock::Create(m_aBlockSaveDate[nCurNum - nCount].pos,
                                               m_aBlockSaveDate[nCurNum - nCount].rot,
                                               m_aBlockSaveDate[nCurNum - nCount].nType);

                                // 変更内容が適用された状態にする
                                m_aBlockSaveDate[nCurNum - nCount].bChangeJudge = true;

                                break;
                            }
                        }

                        // 番号と数を1つ増やす
                        nBlockCount++;
                    }
                }
                // 「変更が適用されたブロックを特定出来た場合」かつ、オブジェクト情報を取得出来ている場合
                else if(bSelectBlockJudge == true && pBlock != NULL) {
                    // 変更内容によって処理を変える
                    switch(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange) {
                            // 座標変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_POS:
                        case DETAILSCHANGE_CHANGE_POS:
                            // 「変更が適用されたブロックの座標・向き・種類・番号が適用する変更内容と一致する場合」
                            if(pBlock->GetPos() == m_aBlockSaveDate[nCurNum - NEXT_DATA].pos && pBlock->GetRot() == m_aBlockSaveDate[nCurNum - NEXT_DATA].rot && pBlock->GetType() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType && pBlock->GetNumber() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber) {

                                bCheckData = true;
                                pBlock->SetPos(m_aBlockSaveDate[nCurNum - NEXT_TIME_DATA].pos);
                            }
                            else {
                                if(m_nLastCountPos == 0) {
                                    nCurNum = ShiftArray(nCurNum);

                                    SaveDate(pBlock->GetPos(),
                                             pBlock->GetRot(),
                                             pBlock->GetNumber(),
                                             DETAILSCHANGE_CHANGE_LAST_POS,
                                             true,
                                             pBlock->GetType(),
                                             nCurNum,
                                             CScene::OBJTYPE_BLOCK);
                                }

                                pBlock->SetPos(m_aBlockSaveDate[nCurNum - NEXT_DATA].pos);
                            }
                            break;

                            // 向き変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_ROT:
                        case DETAILSCHANGE_CHANGE_ROT:
                            // 「変更が適用されたブロックの座標・向き・種類・番号が適用する変更内容と一致する場合」
                            if(pBlock->GetPos() == m_aBlockSaveDate[nCurNum - NEXT_DATA].pos && pBlock->GetRot() == m_aBlockSaveDate[nCurNum - NEXT_DATA].rot && pBlock->GetType() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType && pBlock->GetNumber() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber) {

                                bCheckData = true;
                                pBlock->SetRot(m_aBlockSaveDate[nCurNum - NEXT_TIME_DATA].rot);
                            }
                            else {
                                if(m_nLastCountRot == 0) {
                                    nCurNum = ShiftArray(nCurNum);

                                    SaveDate(pBlock->GetPos(),
                                             pBlock->GetRot(),
                                             pBlock->GetNumber(),
                                             DETAILSCHANGE_CHANGE_LAST_ROT,
                                             true,
                                             pBlock->GetType(),
                                             nCurNum,
                                             CScene::OBJTYPE_BLOCK);
                                }

                                pBlock->SetRot(m_aBlockSaveDate[nCurNum - NEXT_DATA].rot);
                            }
                            break;

                            // 生成の場合
                        case DETAILSCHANGE_CREATE:
                            pBlock->Uninit();
                            break;
                    }
                }
            }
        }

		// オブジェクトタイプが敵の場合
        else if(m_aBlockSaveDate[nCurNum - NEXT_DATA].ObjectType == CScene::OBJTYPE_ENEMY) {
            // 「変更が適用された敵を特定出来た場合」又は、「変更内容が消去の場合」又は、「変更内容が全消去の場合」
            if(bSelectEnemyJudge == true || m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_BREAK || m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                // 変更内容が消去の場合
                if(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_BREAK) {
                    CEnemy::Create(m_aBlockSaveDate[nCurNum - NEXT_DATA].pos,
                                   m_aBlockSaveDate[nCurNum - NEXT_DATA].rot,
                                   m_aBlockSaveDate[nCurNum - NEXT_DATA].nType);
                }
                // 変更内容が全消去の場合
                else if(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                    // ブロックの順番と数を管理する変数
                    int nBlockCount = 0;
                    // ブロックの数が全消去した数以下の間無限にループさせる
                    while(nBlockCount <= m_nAllDeleteNum) {
                        // 全消去した数分配列を確認する
                        for(int nCount = 1; nCount <= m_nAllDeleteNum; nCount++) {
                            // オブジェクトを全消去した瞬間の番号順に生成
                            if(m_aBlockSaveDate[nCurNum - nCount].nNumber == nBlockCount) {
                                CEnemy::Create(m_aBlockSaveDate[nCurNum - nCount].pos,
                                               m_aBlockSaveDate[nCurNum - nCount].rot,
                                               m_aBlockSaveDate[nCurNum - nCount].nType);

                                // 変更内容が適用された状態にする
                                m_aBlockSaveDate[nCurNum - nCount].bChangeJudge = true;

                                break;
                            }
                        }

                        // 番号と数を1つ増やす
                        nBlockCount++;
                    }
                }
                // 「変更が適用された敵を特定出来た場合」かつ、オブジェクト情報を取得出来ている場合
                else if(bSelectEnemyJudge == true && pEnemy != NULL) {
                    // 変更内容によって処理を変える
                    switch(m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange) {
                            // 座標変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_POS:
                        case DETAILSCHANGE_CHANGE_POS:
                            // 「変更が適用された敵の座標・向き・種類・番号が適用する変更内容と一致する場合」
                            if(pEnemy->GetPos() == m_aBlockSaveDate[nCurNum - NEXT_DATA].pos && pEnemy->GetRot() == m_aBlockSaveDate[nCurNum - NEXT_DATA].rot && pEnemy->GetType() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType && pEnemy->GetNumber() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber) {

                                bCheckData = true;
                                pEnemy->SetPos(m_aBlockSaveDate[nCurNum - NEXT_TIME_DATA].pos);
                            }
                            else {
                                if(m_nLastCountPos == 0) {
                                    nCurNum = ShiftArray(nCurNum);

                                    SaveDate(pEnemy->GetPos(),
                                             pEnemy->GetRot(),
                                             pEnemy->GetNumber(),
                                             DETAILSCHANGE_CHANGE_LAST_POS,
                                             true,
                                             pEnemy->GetType(),
                                             nCurNum,
                                             CScene::OBJTYPE_ENEMY);
                                }

                                pEnemy->SetPos(m_aBlockSaveDate[nCurNum - NEXT_DATA].pos);
                            }
                            break;

                            // 向き変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_ROT:
                        case DETAILSCHANGE_CHANGE_ROT:
                            // 「変更が適用された敵の座標・向き・種類・番号が適用する変更内容と一致する場合」
                            if(pEnemy->GetPos() == m_aBlockSaveDate[nCurNum - NEXT_DATA].pos && pEnemy->GetRot() == m_aBlockSaveDate[nCurNum - NEXT_DATA].rot && pEnemy->GetType() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nType && pEnemy->GetNumber() == m_aBlockSaveDate[nCurNum - NEXT_DATA].nNumber) {

                                bCheckData = true;
                                pEnemy->SetRot(m_aBlockSaveDate[nCurNum - NEXT_TIME_DATA].rot);
                            }
                            else {
                                if(m_nLastCountRot == 0) {
                                    nCurNum = ShiftArray(nCurNum);

                                    SaveDate(pEnemy->GetPos(),
                                             pEnemy->GetRot(),
                                             pEnemy->GetNumber(),
                                             DETAILSCHANGE_CHANGE_LAST_ROT,
                                             true,
                                             pEnemy->GetType(),
                                             nCurNum,
                                             CScene::OBJTYPE_ENEMY);
                                }

                                pEnemy->SetRot(m_aBlockSaveDate[nCurNum - NEXT_DATA].rot);
                            }
                            break;

                            // 生成の場合
                        case DETAILSCHANGE_CREATE:
                            pEnemy->Uninit();
                            break;
                    }
                }
            }
        }

		// 「1つ後ろに変更内容が登録されている場合」かつ、
		// 「1つ後ろに変更内容が適用されていない状態の場合」又は、「1つ後ろの変更内容が全消去以外の場合」かつ、
		// 「1つ後ろに変更内容が適用されている場合」
		if (m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange != DETAILSCHANGE_NULL && m_aBlockSaveDate[nCurNum - NEXT_DATA].bChangeJudge == false || m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange == DETAILSCHANGE_TOTALELIMINATION && m_aBlockSaveDate[nCurNum - NEXT_DATA].bChangeJudge == true) {
			// 変更内容が全消去以外の場合
			if (m_aBlockSaveDate[nCurNum - NEXT_DATA].DetailsChange != DETAILSCHANGE_TOTALELIMINATION) {
				if (bCheckData == false) {
					// 変更内容が適用された状態にする
					m_aBlockSaveDate[nCurNum - NEXT_DATA].bChangeJudge = true;
					// 変更情報を保存する変数の配列を管理する変数を-1する
					nCurNum--;
				}
				if (bCheckData == true) {
					// 変更内容が適用された状態にする
					m_aBlockSaveDate[nCurNum - NEXT_DATA].bChangeJudge = true;
					m_aBlockSaveDate[nCurNum - NEXT_TIME_DATA].bChangeJudge = true;
					// 変更情報を保存する変数の配列を管理する変数を-1する
					nCurNum -= NEXT_TIME_DATA;
				}
			}
			// 変更内容が全消去の場合
			else {
				// 全消去した数分配列を確認する
				for (int nCount = 1; nCount <= m_nAllDeleteNum; nCount++) {
					// 変更内容が適用された状態にする
					m_aBlockSaveDate[nCurNum - nCount].bChangeJudge = true;
					// 変更情報を保存する変数の配列を管理する変数を全消去のオブジェクト数分減算する
					nCurNum--;
				}
			}
		}
    }

    // 変更情報を保存する変数の配列を管理する変数を返す
    return nCurNum;
}

//=============================================================================
// 変更内容を元に進める
//=============================================================================
int CCreateStage::RedoUndoOperation(int nCurNum)
{
    // 「現在の配列の1つ後ろが存在している場合」かつ、「現在の配列の1つ後ろの変更が適用されていない状態の場合」かつ、「現在の配列の1つ後ろの変更内容が登録されている場合」
    if(m_aBlockSaveDate[nCurNum].bChangeJudge == true && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL) {
        // 先頭のオブジェクト情報を取得
        CScene *pScene = CScene::GetTopScene();
        CBlock *pBlock;
        CEnemy *pEnemy;
        bool bSelectBlockJudge = false;    // 変更されたブロックオブジェクトを特定出来たかを判定
        bool bSelectEnemyJudge = false;    // 変更された敵オブジェクトを特定出来たかを判定
        bool bCheckData        = false;    // 特定したオブジェクトと変更内容の情報が全て一致しているかを判定
        // 「変更内容が消去以外の場合」かつ、「変更内容が全消去以外の場合」
        if(m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_CREATE && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_TOTALELIMINATION) {
            // 変更が適用されたオブジェクトを捜索する
            for(int nCntScene = 0; nCntScene < CScene::GetNumAll(); nCntScene++) {
                // 中身があるなら
                if(pScene != NULL) {
                    // 次のシーンを記憶
                    CScene *pNextScene = pScene->GetNextScene();

                    // タイプを取得
                    CScene::OBJTYPE objType = pScene->GetObjType();

                    // ブロックなら、
                    if(objType == CScene::OBJTYPE_BLOCK && m_aBlockSaveDate[nCurNum].ObjectType == CScene::OBJTYPE_BLOCK) {
                        // ブロックにキャスト
                        pBlock = (CBlock *)pScene;

                        // 番号を取得
                        int nNumber = pBlock->GetNumber();
                        // 種類を取得
                        int nType = pBlock->GetType();
						// 座標取得
						D3DXVECTOR3 pos = pBlock->GetPos();

                        // ブロックにキャストしたオブジェクトと変更されたオブジェクトの番号が一致する場合
                        if(nNumber == m_aBlockSaveDate[nCurNum].nNumber && nType == m_aBlockSaveDate[nCurNum].nType ||
							nType == m_aBlockSaveDate[nCurNum].nType && pos == m_aBlockSaveDate[nCurNum].pos) {
                            // 特定出来た状態に変更
                            bSelectBlockJudge = true;
                            break;
                        }
                    }

                    // 敵なら、
                    if(objType == CScene::OBJTYPE_ENEMY && m_aBlockSaveDate[nCurNum].ObjectType == CScene::OBJTYPE_ENEMY) {
                        // 敵にキャスト
                        pEnemy = (CEnemy *)pScene;

                        // 番号を取得
                        int nNumber = pEnemy->GetNumber();
                        // 種類を取得
                        int nType = pEnemy->GetType();
						// 座標取得
						D3DXVECTOR3 pos = pEnemy->GetPos();

                        // 敵にキャストしたオブジェクトと変更されたオブジェクトの番号が一致する場合
                        if(nNumber == m_aBlockSaveDate[nCurNum].nNumber && nType == m_aBlockSaveDate[nCurNum].nType ||
							nType == m_aBlockSaveDate[nCurNum].nType && pos == m_aBlockSaveDate[nCurNum].pos) {
                            // 特定出来た状態に変更
                            bSelectEnemyJudge = true;
                            break;
                        }
                    }

                    // 次のシーンにする
                    pScene = pNextScene;
                }
                else {
                    // 中身がないなら、そこで処理を終える
                    break;
                }
            }
        }

		// オブジェクトタイプがブロックの場合
        if(m_aBlockSaveDate[nCurNum].ObjectType == CScene::OBJTYPE_BLOCK) {
            // 「変更が適用されたブロックを特定出来た場合」又は、「変更内容が生成の場合」又は、「変更内容が全消去の場合」
            if(bSelectBlockJudge == true || m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_CREATE || m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                // 変更内容が消去の場合
                if(m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_CREATE) {
                    CBlock::Create(m_aBlockSaveDate[nCurNum].pos,
                                   m_aBlockSaveDate[nCurNum].rot,
                                   m_aBlockSaveDate[nCurNum].nType);
                }
                // 変更内容が全消去の場合
                else if(m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                    m_bUseAllDelete = true;
                }
                // 「変更が適用されたブロックを特定出来た場合」かつ、「オブジェクト情報を取得出来ている場合」
                else if(bSelectBlockJudge == true && pBlock != NULL) {
                    // 変更内容によって処理を変える
                    switch(m_aBlockSaveDate[nCurNum].DetailsChange) {
                            // 座標変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_POS:
                        case DETAILSCHANGE_CHANGE_POS:
                            if(pBlock->GetPos() == m_aBlockSaveDate[nCurNum].pos && pBlock->GetRot() == m_aBlockSaveDate[nCurNum].rot && pBlock->GetType() == m_aBlockSaveDate[nCurNum].nType && pBlock->GetNumber() == m_aBlockSaveDate[nCurNum].nNumber && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL) {

                                bCheckData = true;
                                pBlock->SetPos(m_aBlockSaveDate[nCurNum + NEXT_DATA].pos);
                            }
                            else {
                                pBlock->SetPos(m_aBlockSaveDate[nCurNum].pos);
                            }
                            break;

                            // 向き変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_ROT:
                        case DETAILSCHANGE_CHANGE_ROT:
                            if(pBlock->GetPos() == m_aBlockSaveDate[nCurNum].pos && pBlock->GetRot() == m_aBlockSaveDate[nCurNum].rot && pBlock->GetType() == m_aBlockSaveDate[nCurNum].nType && pBlock->GetNumber() == m_aBlockSaveDate[nCurNum].nNumber && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL) {

                                bCheckData = true;
                                pBlock->SetRot(m_aBlockSaveDate[nCurNum + NEXT_DATA].rot);
                            }
                            else {
                                pBlock->SetRot(m_aBlockSaveDate[nCurNum].rot);
                            }
                            break;

                            // 消去の場合
                        case DETAILSCHANGE_BREAK:
                            pBlock->Uninit();
                            break;
                    }
                }
            }
        }

		// オブジェクトタイプが敵の場合
        else if(m_aBlockSaveDate[nCurNum].ObjectType == CScene::OBJTYPE_ENEMY) {
            // 「変更が適用された敵を特定出来た場合」又は、「変更内容が生成の場合」又は、「変更内容が全消去の場合」
            if(bSelectEnemyJudge == true || m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_CREATE || m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                // 変更内容が消去の場合
                if(m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_CREATE) {
                    CEnemy::Create(m_aBlockSaveDate[nCurNum].pos,
                                   m_aBlockSaveDate[nCurNum].rot,
                                   m_aBlockSaveDate[nCurNum].nType);
                }
                // 変更内容が全消去の場合
                else if(m_aBlockSaveDate[nCurNum].DetailsChange == DETAILSCHANGE_TOTALELIMINATION) {
                    m_bUseAllDelete = true;
                }
                // 「変更が適用されたブロックを特定出来た場合」かつ、「オブジェクト情報を取得出来ている場合」
                else if(bSelectEnemyJudge == true && pEnemy != NULL) {
                    // 変更内容によって処理を変える
                    switch(m_aBlockSaveDate[nCurNum].DetailsChange) {
                            // 座標変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_POS:
                        case DETAILSCHANGE_CHANGE_POS:
                            if(pEnemy->GetPos() == m_aBlockSaveDate[nCurNum].pos && pEnemy->GetRot() == m_aBlockSaveDate[nCurNum].rot && pEnemy->GetType() == m_aBlockSaveDate[nCurNum].nType && pEnemy->GetNumber() == m_aBlockSaveDate[nCurNum].nNumber && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL) {

                                bCheckData = true;
                                pEnemy->SetPos(m_aBlockSaveDate[nCurNum + NEXT_DATA].pos);
                            }
                            else {
                                pEnemy->SetPos(m_aBlockSaveDate[nCurNum].pos);
                            }
                            break;

                            // 向き変更の場合
                        case DETAILSCHANGE_CHANGE_LAST_ROT:
                        case DETAILSCHANGE_CHANGE_ROT:
                            if(pEnemy->GetPos() == m_aBlockSaveDate[nCurNum].pos && pEnemy->GetRot() == m_aBlockSaveDate[nCurNum].rot && pEnemy->GetType() == m_aBlockSaveDate[nCurNum].nType && pEnemy->GetNumber() == m_aBlockSaveDate[nCurNum].nNumber && m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL) {

                                bCheckData = true;
                                pEnemy->SetRot(m_aBlockSaveDate[nCurNum + NEXT_DATA].pos);
                            }
                            else {
                                pEnemy->SetRot(m_aBlockSaveDate[nCurNum].rot);
                            }
                            break;

                            // 消去の場合
                        case DETAILSCHANGE_BREAK:
                            pEnemy->Uninit();
                            break;
                    }
                }
            }
        }

		// 「変更内容が登録されている場合」かつ、「変更内容が適用されている状態の場合」
		if (m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_NULL && m_aBlockSaveDate[nCurNum].bChangeJudge == true) {
			// 変更内容が全消去以外の場合
			if (m_aBlockSaveDate[nCurNum].DetailsChange != DETAILSCHANGE_TOTALELIMINATION) {
				if (bCheckData == false) {
					// 変更内容が適用されていない状態にする
					m_aBlockSaveDate[nCurNum].bChangeJudge = false;
					// 変更情報を保存する変数の配列を管理する変数を+1する
					nCurNum++;
				}
				if (bCheckData == true) {
					// 変更内容が適用されていない状態にする
					m_aBlockSaveDate[nCurNum].bChangeJudge = false;
					m_aBlockSaveDate[nCurNum + NEXT_DATA].bChangeJudge = false;
					// 変更情報を保存する変数の配列を管理する変数を+1する
					nCurNum += NEXT_TIME_DATA;
				}
			}
			// 変更内容が全消去の場合
			else {
				// 全消去した数分配列を確認する
				for (int nCount = 0; nCount < m_nAllDeleteNum; nCount++) {
					// 変更内容が適用されていない状態にする
					m_aBlockSaveDate[nCurNum + nCount].bChangeJudge = false;
					// 変更情報を保存する変数の配列を管理する変数を全消去のオブジェクト数分加算する
					nCurNum++;
				}
			}
		}
    }

    // 変更情報を保存する変数の配列を管理する変数を返す
    return nCurNum;
}

//=============================================================================
// 配列の移転
//=============================================================================
int CCreateStage::ShiftArray(int nCount)
{
    if(nCount >= BLOCK_SAVE_MAX_NUM) {
        for(int nCount = 1; nCount < BLOCK_SAVE_MAX_NUM; nCount++) {
            // 配列の前の情報に配列の現在の情報を代入する
            m_aBlockSaveDate[nCount - NEXT_DATA].pos           = m_aBlockSaveDate[nCount].pos;
            m_aBlockSaveDate[nCount - NEXT_DATA].rot           = m_aBlockSaveDate[nCount].rot;
            m_aBlockSaveDate[nCount - NEXT_DATA].nType         = m_aBlockSaveDate[nCount].nType;
            m_aBlockSaveDate[nCount - NEXT_DATA].bChangeJudge  = m_aBlockSaveDate[nCount].bChangeJudge;
            m_aBlockSaveDate[nCount - NEXT_DATA].DetailsChange = m_aBlockSaveDate[nCount].DetailsChange;
            m_aBlockSaveDate[nCount - NEXT_DATA].nNumber       = m_aBlockSaveDate[nCount].nNumber;
            m_aBlockSaveDate[nCount - NEXT_DATA].ObjectType    = m_aBlockSaveDate[nCount].ObjectType;
        }

        // 最後の情報を消す
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].pos           = D3DXVECTOR3(0, 0, 0);
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].rot           = D3DXVECTOR3(0, 0, 0);
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].nType         = -1;
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].bChangeJudge  = false;
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].DetailsChange = DETAILSCHANGE_NULL;
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].nNumber       = -1;
        m_aBlockSaveDate[BLOCK_SAVE_MAX_NUM - NEXT_DATA].ObjectType    = CScene::OBJTYPE_NONE;

        return BLOCK_SAVE_MAX_NUM - NEXT_DATA;
    }

    return nCount;
}

//=============================================================================
// 変更内容を全て保存
//=============================================================================
void CCreateStage::SaveDate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumber, DETAILSCHANGE DetailsChange, bool bChangeJudge, int nType, int nCount, CScene::OBJTYPE ObjectType)
{
    // 未来の配列内に変更内容が保存されている場合、未来の配列内を全て初期化する
    if(m_aBlockSaveDate[nCount + NEXT_DATA].DetailsChange != DETAILSCHANGE_NULL) {
        InitArray(nCount + NEXT_DATA);
    }

    // 変更内容を保存
    m_aBlockSaveDate[nCount].nType         = nType;
    m_aBlockSaveDate[nCount].bChangeJudge  = bChangeJudge;
    m_aBlockSaveDate[nCount].DetailsChange = DetailsChange;
    m_aBlockSaveDate[nCount].nNumber       = nNumber;
    m_aBlockSaveDate[nCount].ObjectType    = ObjectType;

    // 座標変更の場合
    if(DetailsChange == DETAILSCHANGE_CHANGE_POS || DetailsChange == DETAILSCHANGE_CHANGE_LAST_POS) {
        m_aBlockSaveDate[nCount].pos = pos;
    }
    // 向き変更の場合
    if(DetailsChange == DETAILSCHANGE_CHANGE_ROT || DetailsChange == DETAILSCHANGE_CHANGE_LAST_ROT) {
        m_aBlockSaveDate[nCount].rot = rot;
    }
    // 全消去の場合
    if(DetailsChange == DETAILSCHANGE_TOTALELIMINATION || DetailsChange == DETAILSCHANGE_CREATE || DetailsChange == DETAILSCHANGE_BREAK) {
        m_aBlockSaveDate[nCount].pos = pos;
        m_aBlockSaveDate[nCount].rot = rot;
    }
}

//=============================================================================
// コピー用変数にコピー内容を保存
//=============================================================================
void CCreateStage::SetCopyDate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
    m_CopyPos   = pos;
    m_CopyRot   = rot;
    m_nCopyType = nType;
}

//=============================================================================
// コピー用変数を初期化
//=============================================================================
void CCreateStage::InitCopyDate(void)
{
    m_CopyPos   = D3DXVECTOR3(0, 0, 0);
    m_CopyRot   = D3DXVECTOR3(0, 0, 0);
    m_nCopyType = -1;
}

//=============================================================================
// 配列を初期化
//=============================================================================
void CCreateStage::InitArray(int nCount)
{
    for(int nCOunt = nCount; nCOunt < BLOCK_SAVE_MAX_NUM; nCOunt++) {
        m_aBlockSaveDate[nCOunt].pos           = D3DXVECTOR3(0, 0, 0);
        m_aBlockSaveDate[nCOunt].rot           = D3DXVECTOR3(0, 0, 0);
        m_aBlockSaveDate[nCOunt].nType         = -1;
        m_aBlockSaveDate[nCOunt].bChangeJudge  = false;
        m_aBlockSaveDate[nCOunt].DetailsChange = DETAILSCHANGE_NULL;
        m_aBlockSaveDate[nCOunt].nNumber       = -1;
        m_aBlockSaveDate[nCOunt].ObjectType    = CScene::OBJTYPE_NONE;
    }
}

//=============================================================================
// control + Zで追加された情報数を数える
//=============================================================================
void CCreateStage::CountDetallsChangeLAST(void)
{
    for(int nCount = 0; nCount < BLOCK_SAVE_MAX_NUM; nCount++) {
        if(m_aBlockSaveDate[nCount].DetailsChange == DETAILSCHANGE_CHANGE_LAST_POS) {
            m_nLastCountPos++;
        }

        if(m_aBlockSaveDate[nCount].DetailsChange == DETAILSCHANGE_CHANGE_LAST_ROT) {
            m_nLastCountRot++;
        }
    }
}

//=============================================================================
// 読み込むテキストファイルを取得・セットする
//=============================================================================
HRESULT CCreateStage::LoadTextName(void) {
	// ファイルポイント
	FILE *pFile = NULL;

	// 変数宣言
	int nCntLoad = 0;
	char cReedText[128];    // 文字として読み取り用
	char cHeadText[256];    //
	char cDie[128];

	// ファイル開
	pFile = fopen("data/TXT/LoadTextName.txt", "r");

	// 開けた
	if (pFile != NULL) {
		while (strcmp(cHeadText, "START") != 0) {
			fgets(cReedText, sizeof(cReedText), pFile);
			sscanf(cReedText, "%s", &cHeadText);
		}

		if (strcmp(cHeadText, "START") == 0) {
			while (strcmp(cHeadText, "END") != 0) {
				fgets(cReedText, sizeof(cReedText), pFile);
				sscanf(cReedText, "%s", &cHeadText);

				if (strcmp(cHeadText, "\n") == 0) {
				}
				else if (strcmp(cHeadText, "END") != 0) {
					if (strcmp(cHeadText, "Block") == 0) {
						sscanf(cReedText, "%s %s %s", &cDie, &cDie, &m_cTxtName[0][0]);
					}

					if (strcmp(cHeadText, "Enemy") == 0) {
						sscanf(cReedText, "%s %s %s", &cDie, &cDie, &m_cTxtName[1][0]);
					}

					if (strcmp(cHeadText, "Map") == 0) {
						sscanf(cReedText, "%s %s %s", &cDie, &cDie, &m_cTxtName[2][0]);

						nCntLoad++;
					}
				}
			}
		}
		// ファイル閉
		fclose(pFile);

		return S_OK;
	}

	// 開けない
	else {
		printf("開けれませんでした\n");
	}

	return E_FAIL;
}

//=============================================================================
// 選択したオブジェクトタイプが使用出来るかを判定
//=============================================================================
void CCreateStage::SetObjectType(int nType) {
	if (m_CreateType == CREATEOBJECT_STAGE && CBlock::GetBlockData(nType).bUseJudge == true) {
		m_nBlockType = nType;
	}
	else if (m_CreateType == CREATEOBJECT_ENEMY && CEnemy::GetEnemyData(nType).bUseJudge == true) {
		m_nEnemyType = nType;
	}
}
