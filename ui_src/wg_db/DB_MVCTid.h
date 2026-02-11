#ifndef __DB_MVCTid_DB_H__
#define __DB_MVCTid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVCTid
{
public:
	CDB_MVCTid();
	virtual ~CDB_MVCTid();
	CDBDoc* m_pDoc;

public:
	void Add(T_MVCTid_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MVCTid_D& rData);
		//{return m_mvctid.Lookup(1,rData);}
	int GetCount();
		//{return m_mvctid.GetCount();}

protected:
	CMap<T_MVCTid_K,T_MVCTid_K,T_MVCTid_D,T_MVCTid_D&>m_mvctid;
};

#endif