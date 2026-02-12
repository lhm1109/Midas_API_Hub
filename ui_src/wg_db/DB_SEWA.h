#pragma once

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_SEWA
{
public:
	CDB_SEWA();
	virtual ~CDB_SEWA();
	CDBDoc* m_pDoc;

public:
	void Add(T_SEWA_K Key, T_SEWA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SEWA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SEWA_K Key, T_SEWA_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SEWA_K& rKey, T_SEWA_D& rData);

protected:
	CMap<T_SEWA_K, T_SEWA_K, T_SEWA_D, T_SEWA_D&> m_SEWA;
};
