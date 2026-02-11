#ifndef __DB_SCMT_DB_H__
#define __DB_SCMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SCMT
{
public:
	CDB_SCMT();
	virtual ~CDB_SCMT();
	CDBDoc* m_pDoc;

public:
	void Add(T_SCMT_K Key,T_SCMT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SCMT_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SCMT_K Key,T_SCMT_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SCMT_K& rKey,T_SCMT_D& rData);

protected:
	CMap<T_SCMT_K,T_SCMT_K,T_SCMT_D,T_SCMT_D&>m_SCMT;
};

#endif