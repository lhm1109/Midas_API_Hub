#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*      Russia Rcs Transverse reinforcement : T_RRTR_D					*/
/************************************************************************/
class CDBDoc;
class CDB_ELEM;
class CDB_RRTR
{
public:
	CDB_RRTR();
	virtual ~CDB_RRTR();
	CDBDoc* m_pDoc;

public:
	void Add(T_RRTR_K Key, T_RRTR_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_RRTR_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_RRTR_K Key, T_RRTR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_RRTR_K& rKey, T_RRTR_D& rData);
protected:
	CMap<T_RRTR_K, T_RRTR_K, T_RRTR_D, T_RRTR_D&> m_rrtr;
};

