#define _CRT_SECURE_NO_WARNINGS

//===============================================
//
// フィールド処理 (block.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "enemy.h"

#include "manager.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================
LPD3DXMESH CEnemy::m_apMesh[CEnemy::TYPE_MAX] = {};
LPD3DXBUFFER CEnemy::m_apBuffMat[CEnemy::TYPE_MAX] = {};
DWORD CEnemy::m_aNumMat[CEnemy::TYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[CEnemy::TYPE_MAX] = {};
CEnemy::ENEMYDATA CEnemy::m_aEnemyData[TYPE_MAX] = {};
char CEnemy::m_cTextureName[TYPE_MAX][256] = {};                // テクスチャパス保管用変数
char CEnemy::m_cModelName[TYPE_MAX][256] = {};                  // モデルパス保管用変数

CEnemy* CEnemy::m_pSelectEnemy = NULL;
int CEnemy::m_nNumAll = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CEnemy::CEnemy()
	: CScene3D(CScene::OBJTYPE_ENEMY)
{
	m_collisionSize = DEFAULT_VECTOR;
	m_nType = TYPE_1;
	m_bWallRun = false;

	m_bBlink = false;
	m_nCntTime = 0;
	m_nNumber = m_nNumAll;

	m_nNumAll++;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
	m_nNumAll--;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nType)
{
	CScene3D::Init(pos, size, rot);

	SetScale(size);

	// メッシュ、テクスチャをバインド
	BindMesh(m_apMesh[m_nType], m_apBuffMat[m_nType], m_aNumMat[m_nType]);
	BindTexture(m_apTexture[m_nType]);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 0より大きいなら、減らす
	if (m_nCntTime > 0) {
		m_nCntTime--;
	}

	// 選択しているブロックを点滅させる
	if (this == m_pSelectEnemy) {
		if (m_nCntTime == 0) {
			m_bBlink = !m_bBlink;

			m_nCntTime = BLINK_TIME;
		}
	}
	else {
		m_nCntTime = 0;

		m_bBlink = false;
	}

	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_bBlink == false) {
		CScene3D::Draw();
	}
}

//=============================================================================
// モデルデータ読み込み処理
//=============================================================================
HRESULT CEnemy::Load(void)
{
	LoadTxt();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCount = 0; nCount < TYPE_MAX; nCount++) {
		if (m_aEnemyData[nCount].bUseJudge == true) {
			D3DXLoadMeshFromX(LPCSTR(m_aEnemyData[nCount].ModelPath), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_apBuffMat[m_aEnemyData[nCount].Type], NULL, &m_aNumMat[m_aEnemyData[nCount].Type], &m_apMesh[m_aEnemyData[nCount].Type]);

			if (strcmp(m_aEnemyData[nCount].TexturePath, "NULL") != 0 && strcmp(m_aEnemyData[nCount].TexturePath, " ") != 0) {
				D3DXCreateTextureFromFile(pDevice, m_aEnemyData[nCount].TexturePath, &m_apTexture[m_aEnemyData[nCount].Type]);
			}
		}
	}

	return S_OK;
}

//=============================================================================
// オブジェクト情報が記されたテキストファイルを読み込む
//=============================================================================
void CEnemy::LoadTxt(void)
{
	// ファイルポイント
	FILE* pFile = NULL;

	// 変数宣言
	int nCntLoad = 0;
	int nTextureCount = 0;
	int nModelCount = 0;
	char cReedText[128];    // 文字として読み取り用
	char cHeadText[256];    //
	char cDie[128];

	// デバッグ用の変数
	char cDrawName[TYPE_MAX][256];
	char cTextureNumber[TYPE_MAX][256];
	int nModelNumber[TYPE_MAX];
	D3DXVECTOR3 Game_Collision_Siz[TYPE_MAX];
	D3DXVECTOR3 Toul_Collision_Siz[TYPE_MAX];

	// ファイル開
	pFile = fopen(LOAD_ENEMY_TXT, "r");

	// 開けた
	if (pFile != NULL) {
		while (strcmp(cHeadText, "OBJECTMATCH") != 0) {
			fgets(cReedText, sizeof(cReedText), pFile);
			sscanf(cReedText, "%s", &cHeadText);

			if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0) {
				sscanf(cReedText, "%s %s %s", &cDie, &cDie, &m_cTextureName[nTextureCount][0]);

				nTextureCount++;
			}

			if (strcmp(cHeadText, "MODEL_FILENAME") == 0) {
				sscanf(cReedText, "%s %s %s", &cDie, &cDie, &m_cModelName[nModelCount][0]);

				nModelCount++;
			}
		}

		if (strcmp(cHeadText, "OBJECTMATCH") == 0) {
			while (strcmp(cHeadText, "END_SCRIPT") != 0) {
				fgets(cReedText, sizeof(cReedText), pFile);
				sscanf(cReedText, "%s", &cHeadText);

				if (strcmp(cHeadText, "\n") == 0) {
				}
				else if (strcmp(cHeadText, "END_OBJECTMATCH") != 0) {
					if (strcmp(cHeadText, "DRAW_NAME") == 0) {
						sscanf(cReedText, "%s %s %s", &cDie, &cDie, &cDrawName[nCntLoad][0]);
					}

					if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0) {
						sscanf(cReedText, "%s %s %s", &cDie, &cDie, &cTextureNumber[nCntLoad][0]);
					}

					if (strcmp(cHeadText, "MODEL_FILENAME") == 0) {
						sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nModelNumber[nCntLoad]);
					}
					if (strcmp(cHeadText, "TOUL_COLLISION_SIZ") == 0) {
						sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &Toul_Collision_Siz[nCntLoad].x, &Toul_Collision_Siz[nCntLoad].y, &Toul_Collision_Siz[nCntLoad].z);
					}

					if (strcmp(cHeadText, "GAME_COLLISION_SIZ") == 0) {
						sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &Game_Collision_Siz[nCntLoad].x, &Game_Collision_Siz[nCntLoad].y, &Game_Collision_Siz[nCntLoad].z);

						SetEnemyData(&m_cModelName[nModelNumber[nCntLoad]][0], &m_cTextureName[atoi(cTextureNumber[nCntLoad])][0], &cDrawName[nCntLoad][0], (TYPE)nCntLoad, Game_Collision_Siz[nCntLoad], Toul_Collision_Siz[nCntLoad], nCntLoad, true);

						nCntLoad++;
					}
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else {
		printf("開けれませんでした\n");
	}
}

//=============================================================================
// モデルデータ破棄処理
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCnt = 0; nCnt < CEnemy::TYPE_MAX; nCnt++) {
		if (m_apMesh[nCnt] != NULL) {
			m_apMesh[nCnt]->Release();
			m_apMesh[nCnt] = NULL;
		}
		if (m_apBuffMat[nCnt] != NULL) {
			m_apBuffMat[nCnt]->Release();
			m_apBuffMat[nCnt] = NULL;
		}
		if (m_apTexture[nCnt] != NULL) {
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CEnemy* pEnemy = NULL;
	pEnemy = new CEnemy;
	pEnemy->m_nType = nType;
	pEnemy->Init(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f), rot, nType);
	pEnemy->m_collisionSize = m_aEnemyData[nType].Toul_Collision_Siz;

	return pEnemy;
}

//=============================================================================
// テキストファイルから読み取った情報を保持する
//=============================================================================
HRESULT CEnemy::SetEnemyData(char* ModelPath, char* TexturePath, char* DrawName, TYPE Type, D3DXVECTOR3 Game_Collision_Siz, D3DXVECTOR3 Toul_Collision_Siz, int nNum, bool bUseJudge)
{
	strcpy(m_aEnemyData[nNum].ModelPath, ModelPath);
	if (strcmp(TexturePath, "NULL") != 0 && strcmp(TexturePath, " ") != 0 && strcmp(TexturePath, "0") != 0) {
		strcpy(m_aEnemyData[nNum].TexturePath, TexturePath);
	}
	strcpy(m_aEnemyData[nNum].DrawName, DrawName);
	m_aEnemyData[nNum].Type = Type;
	m_aEnemyData[nNum].Game_Collision_Siz = Game_Collision_Siz;
	m_aEnemyData[nNum].Toul_Collision_Siz = Toul_Collision_Siz;
	m_aEnemyData[nNum].bUseJudge = false;

	if (strcmp(m_aEnemyData[nNum].ModelPath, ModelPath) == 0 && strcmp(m_aEnemyData[nNum].TexturePath, TexturePath) == 0 ||
		strcmp(m_aEnemyData[nNum].ModelPath, ModelPath) == 0 && strcmp(m_aEnemyData[nNum].TexturePath, "NULL") != 0 || strcmp(m_aEnemyData[nNum].TexturePath, " ") != 0) {
		m_aEnemyData[nNum].bUseJudge = bUseJudge;

		return S_OK;
	}

	return E_FAIL;
}