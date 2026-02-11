#pragma once

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_SESP
{
public:
	CDB_SESP();
	virtual ~CDB_SESP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SESP_K Key, T_SESP_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SESP_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SESP_K Key, T_SESP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SESP_K& rKey, T_SESP_D& rData);

protected:
	CMap<T_SESP_K, T_SESP_K, T_SESP_D, T_SESP_D&> m_SESP;
};
