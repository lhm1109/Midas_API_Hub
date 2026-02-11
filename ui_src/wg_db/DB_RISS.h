#ifndef __DB_RISS_DB_H__
#define __DB_RISS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CDBDoc;
class CDB_ELEM;
class CDB_RISS
{
public:
	CDB_RISS();
	virtual ~CDB_RISS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RISS_K Key,T_RISS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RISS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RISS_K Key,T_RISS_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_RISS_K& rKey,T_RISS_D& rData);

protected:
	CMap<T_RISS_K,T_RISS_K,T_RISS_D,T_RISS_D&>m_RISS;
};

#endif