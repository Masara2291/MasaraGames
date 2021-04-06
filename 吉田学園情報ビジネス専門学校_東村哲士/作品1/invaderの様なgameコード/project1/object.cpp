#include "object.h"
#include "scene2d.h"
#include "renderer.h"

CObject* CObject::m_apScene[MAX_PRIORITY_NUM][MAX_NUM] = {};
int CObject::m_nNumAll = 0;

CObject::CObject(int nPriority) {
	for (int nCount = 0; nCount < MAX_NUM; nCount++) {
		if (m_apScene[nPriority][nCount] == NULL) {
			m_apScene[nPriority][nCount] = this;
			m_nPriority = nPriority;
			m_nNumber = nCount;
			m_nNumAll++;

			break;
		}
	}
}

CObject::~CObject() {
	m_nNumAll--;
}

void CObject::UpdateAll(void) {
	for (int nCount_Priority = 0; nCount_Priority < MAX_PRIORITY_NUM; nCount_Priority++) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			if (m_apScene[nCount_Priority][nCount] != NULL) {
				m_apScene[nCount_Priority][nCount]->Update();
			}
		}
	}
}

void CObject::DrawAll(void) {
	for (int nCount_Priority = 0; nCount_Priority < MAX_PRIORITY_NUM; nCount_Priority++) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			if (m_apScene[nCount_Priority][nCount] != NULL) {
				m_apScene[nCount_Priority][nCount]->Draw();
			}
		}
	}
}

void CObject::UninitAll(void) {
	for (int nCount_Priority = 0; nCount_Priority < MAX_PRIORITY_NUM; nCount_Priority++) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			if (m_apScene[nCount_Priority][nCount] != NULL) {
				m_apScene[nCount_Priority][nCount]->Uninit();
				m_apScene[nCount_Priority][nCount] = NULL;
			}
		}
	}
}

void CObject::UninitSome(OBJTYPE ObjType, bool bJudge) {
	if (bJudge == true) {
		for (int nCount_Priority = 0; nCount_Priority < MAX_PRIORITY_NUM; nCount_Priority++) {
			for (int nCount = 0; nCount < MAX_NUM; nCount++) {
				if (m_apScene[nCount_Priority][nCount] != NULL && m_apScene[nCount_Priority][nCount]->m_ObjType != ObjType) {
					m_apScene[nCount_Priority][nCount]->Uninit();
					m_apScene[nCount_Priority][nCount] = NULL;
				}
			}
		}
	}

	if (bJudge == false) {
		for (int nCount = 0; nCount < MAX_NUM; nCount++) {
			if (m_apScene[(int)ObjType][nCount] != NULL) {
				m_apScene[(int)ObjType][nCount]->Uninit();
				m_apScene[(int)ObjType][nCount] = NULL;
			}
		}
	}
}

void CObject::Release(void) {
	int nNum;
	int nPriority;

  	if (m_apScene[m_nPriority][m_nNumber] != NULL) {
		nNum = m_nNumber;
		nPriority = m_nPriority;
		delete m_apScene[nPriority][nNum];
		m_apScene[nPriority][nNum] = NULL;
	}
}

void CObject::SetObjType(OBJTYPE ObjType) {
	m_ObjType = ObjType;
}

CObject::OBJTYPE CObject::GetObjType(void)
{
	return m_ObjType;
}

CObject* CObject::GetScene(int nPriority, int nNum) {
	return m_apScene[nPriority][nNum];
}