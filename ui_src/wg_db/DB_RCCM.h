#ifndef __DB_RCCM_DB_H__
#define __DB_RCCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_RCCM
{
public:
	CDB_RCCM();
	virtual ~CDB_RCCM();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCCM_K Key,T_RCCM_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RCCM_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RCCM_K Key,T_RCCM_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_RCCM_K& rKey,T_RCCM_D& rData);

protected:
	CMap<T_RCCM_K,T_RCCM_K,T_RCCM_D,T_RCCM_D&>m_RCCM;
};

#endif