#ifndef __DB_ULFS_DB_H__
#define __DB_ULFS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LCOM;

class CDB_ULFS
{
public:
	CDB_ULFS();
	virtual ~CDB_ULFS();
	CDBDoc* m_pDoc;

public:
	T_ULFS_K m_nStartNum;
	T_ULFS_K m_nLastNum;

public:
	void Add(T_ULFS_K Key,T_ULFS_D& rData);
	BOOL Del(T_ULFS_K Key);

public:
	BOOL Get(T_ULFS_K Key,T_ULFS_D& rData);
		//{return m_ulfs.Lookup(Key,rData);}
	int GetCount();
		//{return m_ulfs.GetCount();}
	POSITION GetStart();
		//{return m_ulfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ULFS_K& rKey,T_ULFS_D& rData);
		//{m_ulfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ULFS_K,T_ULFS_K,T_ULFS_D,T_ULFS_D&>m_ulfs;
};

#endif