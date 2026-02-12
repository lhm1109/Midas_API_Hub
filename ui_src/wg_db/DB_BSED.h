#ifndef __DB_BSED_DB_H__
#define __DB_BSED_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_BSED
{
public:
	CDB_BSED();
	virtual ~CDB_BSED();
	CDBDoc* m_pDoc;

public:
	void Add(T_BSED_D& rData);
	BOOL Del();

public:
	BOOL Get(T_BSED_D& rData);
	BOOL Get(T_BSED_K Key,T_BSED_D& rData);
		//{return m_bsed.Lookup(1,rData);}
	int GetCount();
		//{return m_bsed.GetCount();}

protected:
	CMap<T_BSED_K,T_BSED_K,T_BSED_D,T_BSED_D&>m_bsed;
};

#endif