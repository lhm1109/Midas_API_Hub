#pragma once

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_SECO
{
public:
	CDB_SECO();
	virtual ~CDB_SECO();
	CDBDoc* m_pDoc;

public:
	void Add(T_SECO_K Key, T_SECO_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SECO_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SECO_K Key, T_SECO_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SECO_K& rKey, T_SECO_D& rData);

protected:
	CMap<T_SECO_K, T_SECO_K, T_SECO_D, T_SECO_D&> m_SECO;
};
