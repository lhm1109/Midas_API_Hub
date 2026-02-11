#ifndef __DB_JDSH_DB_H__
#define __DB_JDSH_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STOR;
class CDB_JDSH
{
public:
	CDB_JDSH();
	virtual ~CDB_JDSH();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_JDSH_K Key,T_JDSH_D& rData,CDB_STOR* pStor);
	BOOL Del(T_JDSH_K Key,CDB_STOR* pStor);
	
public:
	BOOL Get(T_JDSH_K Key,T_JDSH_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_JDSH_K& rKey,T_JDSH_D& rData);
	
protected:
	CMap<T_JDSH_K,T_JDSH_K,T_JDSH_D,T_JDSH_D&>m_JDSH;
};

#endif