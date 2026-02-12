#pragma once

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_HLFW
{
public:
	CDB_HLFW();
	virtual ~CDB_HLFW();
	CDBDoc* m_pDoc;

public:
	void Add(T_HLFW_K Key, T_HLFW_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_HLFW_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_HLFW_K Key, T_HLFW_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_HLFW_K& rKey, T_HLFW_D& rData);

protected:
	CMap<T_HLFW_K, T_HLFW_K, T_HLFW_D, T_HLFW_D&> m_HLFW;
};
