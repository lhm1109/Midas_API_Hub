#ifndef __DB_BPLT_DB_H__
#define __DB_BPLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BPLT
{
public:
	CDB_BPLT();
	virtual ~CDB_BPLT();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_BPLT_K Key,T_BPLT_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_BPLT_K Key,CDB_ELEM* pElem);
	
public:
	BOOL Get(T_BPLT_K Key,T_BPLT_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_BPLT_K& rKey,T_BPLT_D& rData);
	
protected:
	CMap<T_BPLT_K,T_BPLT_K,T_BPLT_D,T_BPLT_D&>m_BPLT;
};

#endif