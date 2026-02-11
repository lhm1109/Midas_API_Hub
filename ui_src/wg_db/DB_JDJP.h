#ifndef __DB_JDJP_DB_H__
#define __DB_JDJP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_NODE;
class CDB_JDJP
{
public:
	CDB_JDJP();
	virtual ~CDB_JDJP();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_JDJP_K Key,T_JDJP_D& rData,CDB_NODE* pStor);
	BOOL Del(T_JDJP_K Key,CDB_NODE* pStor);
	
public:
	BOOL Get(T_JDJP_K Key,T_JDJP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_JDJP_K& rKey,T_JDJP_D& rData);
	
protected:
	CMap<T_JDJP_K,T_JDJP_K,T_JDJP_D,T_JDJP_D&>m_JDJP;
};

#endif