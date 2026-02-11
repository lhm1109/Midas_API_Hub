#ifndef __DB_LDFS_DB_H__
#define __DB_LDFS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LDFS
{
public:
	CDB_LDFS();
	virtual ~CDB_LDFS();
	CDBDoc* m_pDoc;

public:
	T_LDFS_K m_nStartNum;
	T_LDFS_K m_nLastNum;

public:
	void Add(T_LDFS_K Key,T_LDFS_D& rData);
	BOOL Del(T_LDFS_K Key);

public:
	BOOL Get(T_LDFS_K Key,T_LDFS_D& rData)
		{return m_ldfs.Lookup(Key,rData);}
	int GetCount()
		{return m_ldfs.GetCount();}
	POSITION GetStart()
		{return m_ldfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LDFS_K& rKey,T_LDFS_D& rData)
		{m_ldfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LDFS_K,T_LDFS_K,T_LDFS_D,T_LDFS_D&>m_ldfs;
};

#endif