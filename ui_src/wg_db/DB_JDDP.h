#ifndef __DB_JDDP_DB_H__
#define __DB_JDDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_NODE;
class CDB_JDDP
{
public:
	CDB_JDDP();
	virtual ~CDB_JDDP();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_JDDP_K Key,T_JDDP_D& rData,CDB_NODE* pStor);
	BOOL Del(T_JDDP_K Key,CDB_NODE* pStor);
	
public:
	BOOL Get(T_JDDP_K Key,T_JDDP_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_JDDP_K& rKey,T_JDDP_D& rData);
	
protected:
	CMap<T_JDDP_K,T_JDDP_K,T_JDDP_D,T_JDDP_D&>m_JDDP;
};

#endif