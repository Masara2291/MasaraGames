#include "math.h"
#include "CreateStage.h"
#include "enemy.h"
#include "player.h"

CCreateStage::ENEMYDATE CMath::GetEnemyDate(D3DXVECTOR3 pos, CManager::PRIORITY nPriority,
	D3DXVECTOR2 n01, D3DXVECTOR2 n02,
	D3DXVECTOR2 n03, D3DXVECTOR2 n04,
	D3DXVECTOR2 nAdd01, D3DXVECTOR2 nAdd02,
	D3DXVECTOR2 nAdd03, D3DXVECTOR2 nAdd04) {

	CCreateStage::ENEMYDATE m_EnemyDate = {};
	D3DXVECTOR3 Judge_pos;

	//クリックで敵のタイプを取得->置くときのタイプに切り替える
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(nPriority, nCount);
		if (pObject != NULL) {
			CObject::OBJTYPE ObjType = pObject->GetObjType();
			Judge_pos = ((CEnemy*)pObject)->GetPos();

			if (pos.x + ((CURSOR_SIZ + nAdd01.x) / n01.x) >= Judge_pos.x - ((ENEMY_SIZ + nAdd03.x) / n03.x) &&
				pos.x - ((CURSOR_SIZ + nAdd02.x) / n02.x) <= Judge_pos.x + ((ENEMY_SIZ + nAdd04.x) / n04.x) &&
				pos.y - ((CURSOR_SIZ + nAdd01.y) / n01.y) >= Judge_pos.y - ((ENEMY_SIZ + nAdd03.y) / n03.y) &&
				pos.y - ((CURSOR_SIZ + nAdd02.y) / n02.y) <= Judge_pos.y + ((ENEMY_SIZ + nAdd04.y) / n04.y)) {

				m_EnemyDate.nType = ((CEnemy*)pObject)->GetEnemyType();
				m_EnemyDate.pos = Judge_pos;
			}
		}
	}

	return m_EnemyDate;
}

bool CMath::GetOverlapJudge(D3DXVECTOR3 pos, CManager::PRIORITY nPriority, 
	D3DXVECTOR2 n01, D3DXVECTOR2 n02,
	D3DXVECTOR2 n03, D3DXVECTOR2 n04,
	D3DXVECTOR2 nAdd01, D3DXVECTOR2 nAdd02,
	D3DXVECTOR2 nAdd03, D3DXVECTOR2 nAdd04) {

	D3DXVECTOR3 Judge_pos;

	//クリックで敵のタイプを取得->置くときのタイプに切り替える
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(nPriority, nCount);
		if (pObject != NULL) {
			CObject::OBJTYPE ObjType = pObject->GetObjType();
			Judge_pos = ((CEnemy*)pObject)->GetPos();

			if (pos.x + ((CURSOR_SIZ + nAdd01.x) / n01.x) >= Judge_pos.x - ((ENEMY_SIZ + nAdd03.x) / n03.x) &&
				pos.x - ((CURSOR_SIZ + nAdd02.x) / n02.x) <= Judge_pos.x + ((ENEMY_SIZ + nAdd04.x) / n04.x) &&
				pos.y - ((CURSOR_SIZ + nAdd01.y) / n01.y) >= Judge_pos.y - ((ENEMY_SIZ + nAdd03.y) / n03.y) &&
				pos.y - ((CURSOR_SIZ + nAdd02.y) / n02.y) <= Judge_pos.y + ((ENEMY_SIZ + nAdd04.y) / n04.y)) {

				return true;
			}
		}
	}

	return false;
}

CObject* CMath::GetObjectDate(D3DXVECTOR3 pos, CManager::PRIORITY nPriority, 
	D3DXVECTOR2 n01, D3DXVECTOR2 n02,
	D3DXVECTOR2 n03, D3DXVECTOR2 n04,
	D3DXVECTOR2 nAdd01, D3DXVECTOR2 nAdd02,
	D3DXVECTOR2 nAdd03, D3DXVECTOR2 nAdd04) {

	D3DXVECTOR3 Judge_pos;

	//クリックで敵のタイプを取得->置くときのタイプに切り替える
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		CObject* pObject = CObject::GetScene(nPriority, nCount);
		if (pObject != NULL) {
			CObject::OBJTYPE ObjType = pObject->GetObjType();
			Judge_pos = ((CEnemy*)pObject)->GetPos();

			if (pos.x + ((CURSOR_SIZ + nAdd01.x) / n01.x) >= Judge_pos.x - ((ENEMY_SIZ + nAdd03.x) / n03.x) &&
				pos.x - ((CURSOR_SIZ + nAdd02.x) / n02.x) <= Judge_pos.x + ((ENEMY_SIZ + nAdd04.x) / n04.x) &&
				pos.y - ((CURSOR_SIZ + nAdd01.y) / n01.y) >= Judge_pos.y - ((ENEMY_SIZ + nAdd03.y) / n03.y) &&
				pos.y - ((CURSOR_SIZ + nAdd02.y) / n02.y) <= Judge_pos.y + ((ENEMY_SIZ + nAdd04.y) / n04.y)) {

				return pObject;
			}
		}
	}

	return NULL;
}

bool CMath::CollisionDetection(D3DXVECTOR3 pos01, D3DXVECTOR3 pos02, D3DXVECTOR3 siz01, D3DXVECTOR3 siz02, 
	D3DXVECTOR2 n01, D3DXVECTOR2 n02,
	D3DXVECTOR2 n03, D3DXVECTOR2 n04,
	D3DXVECTOR2 nAdd01, D3DXVECTOR2 nAdd02,
	D3DXVECTOR2 nAdd03, D3DXVECTOR2 nAdd04) {

	if (pos01.x + ((siz01.x + nAdd01.x) / n01.x) >= pos02.x - ((siz02.x + nAdd03.x) / n03.x) &&
		pos01.x - ((siz01.x + nAdd02.x) / n02.x) <= pos02.x + ((siz02.x + nAdd04.x) / n04.x) &&
		pos01.y + ((siz01.y + nAdd01.y) / n01.y) >= pos02.y - ((siz02.y + nAdd03.y) / n03.y) &&
		pos01.y - ((siz01.y + nAdd02.y) / n02.y) <= pos02.y + ((siz02.y + nAdd04.y) / n04.y)) {
		
		return true;
	}

	return false;
}

bool CMath::CircularityJudgment(D3DXVECTOR3 siz01, D3DXVECTOR3 siz02, D3DXVECTOR3 pos01, D3DXVECTOR3 pos02) {
	CObject* pObject = CObject::GetScene(CManager::PRIORITY_PLAYER, 0);

	float fdistance = sqrtf(powf(((CPlayer*)pObject)->GetPos().x - pos02.x, 2) + powf(((CPlayer*)pObject)->GetPos().y - pos02.y, 2));

	if (fdistance <= (siz01.x  /2) + (siz02.x / 2)) {
		return true;
	}

	return false;
}