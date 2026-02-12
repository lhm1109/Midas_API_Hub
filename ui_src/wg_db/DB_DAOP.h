#ifndef __DB_DAOP_DB_H__
#define __DB_DAOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DAOP
{
public:
	CDB_DAOP();
	virtual ~CDB_DAOP();
	CDBDoc* m_pDoc;

public:
	void Add(T_DAOP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DAOP_D& rData);
		//{return m_daop.Lookup(1,rData);}
	int GetCount();
		//{return m_daop.GetCount();}

protected:
	CMap<T_DAOP_K,T_DAOP_K,T_DAOP_D,T_DAOP_D&>m_daop;
};

#endif