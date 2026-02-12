#pragma once

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_SEBE
{
public:
	CDB_SEBE();
	virtual ~CDB_SEBE();
	CDBDoc* m_pDoc;

public:
	void Add(T_SEBE_K Key, T_SEBE_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SEBE_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SEBE_K Key, T_SEBE_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SEBE_K& rKey, T_SEBE_D& rData);

protected:
	CMap<T_SEBE_K, T_SEBE_K, T_SEBE_D, T_SEBE_D&> m_SEBE;
};
