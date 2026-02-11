#ifndef __DB_DCFS_DB_H__
#define __DB_DCFS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DCFS
{
public:
	CDB_DCFS();
	virtual ~CDB_DCFS();
	CDBDoc* m_pDoc;

public:
	void Add(T_DCFS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DCFS_D& rData);
		//{return m_dcfs.Lookup(1,rData);}
	int GetCount();
		//{return m_dcfs.GetCount();}

protected:
	CMap<T_DCFS_K,T_DCFS_K,T_DCFS_D,T_DCFS_D&>m_dcfs;
};

#endif