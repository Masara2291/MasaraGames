#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

#define NUM_VERTEX (4)//í∏ì_êî
#define NUM_POLYGON (2)//É|ÉäÉSÉìêî
#define MAX_NUM (1024)//É|ÉäÉSÉìç≈ëÂêî
#define PLAYER_NUM (1)
#define MAX_PRIORITY_NUM (10) //óDêÊèáà ÇÃêî

class CObject {
public:
	typedef enum {
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER_DRAW,
		OBJTYPE_PLAYER_JUDGE,
		OBJTYPE_ENEMY,
		OBJTYPE_BOSS,
		OBJTYPE_ENEMY_CREAT,
		OBJTYPE_BULLET,
		OBJTYPE_BG,
		OBJTYPE_PLAYER_BULLET,
		OBJTYPE_ENEMY_BULLET,
		OBJTYPE_MAX
	} OBJTYPE;

	CObject(int nPriority = 3);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);
	static void DrawAll(void);
	static void UninitAll(void);
	static void UninitSome(OBJTYPE ObjType, bool bJudge);

	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	static CObject* GetScene(int nPriority, int nNum);

protected:
	void Release(void);

private:
	static CObject* m_apScene[MAX_PRIORITY_NUM][MAX_NUM];
	static int m_nNumAll;
	int m_nNumber;
	OBJTYPE m_ObjType;
	int m_nPriority;
};

#endif