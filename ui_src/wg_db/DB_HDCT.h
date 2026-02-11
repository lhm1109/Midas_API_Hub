#ifndef __DB_HDCT_DB_H__
#define __DB_HDCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_HDCT
{
public:
	CDB_HDCT();
	virtual ~CDB_HDCT();
	CDBDoc* m_pDoc;

public:
	void Add(T_HDCT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_HDCT_D& rData);
		//{return m_hdct.Lookup(1,rData);}
	int GetCount();
		//{return m_hdct.GetCount();}

protected:
	CMap<T_HDCT_K,T_HDCT_K,T_HDCT_D,T_HDCT_D&>m_hdct;
};

#endif