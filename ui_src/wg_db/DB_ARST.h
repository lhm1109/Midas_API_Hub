#pragma once
#include "DB_ST_DT.h"
/************************************************************************/
/*      Allocate Reinforced Sect : ARST		                            */
/************************************************************************/
class CDBDoc;
class CDB_MATL;
class CDB_RFST;
class CDB_ELEM;
class CDB_ARST
{
public:
	CDB_ARST();
	virtual ~CDB_ARST();
	CDBDoc* m_pDoc;

public:
	void Add(T_ARST_K Key, T_ARST_D& rData, CDB_MATL* pMatl, CDB_RFST* pRfst, CDB_ELEM* pElem);
	BOOL Del(T_ARST_K Key, CDB_MATL* pMatl, CDB_RFST* pRfst, CDB_ELEM* pElem);

public:
	BOOL Get(T_ARST_K Key,T_ARST_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_ARST_K& rKey, T_ARST_D& rData);
	BOOL IsUsedRfst(T_RFST_K RfstKey);
protected:
	CMap<T_RFST_K, T_RFST_K, int, int> m_rfstCount;
	CMap<T_ARST_K, T_ARST_K, T_ARST_D, T_ARST_D&> m_arst;
};

