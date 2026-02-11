#ifndef __DB_PSCD_DB_H__
#define __DB_PSCD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PSCD
{
public:
	CDB_PSCD();
	virtual ~CDB_PSCD();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSCD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PSCD_D& rData);
		//{return m_pscd.Lookup(1,rData);}
	int GetCount();
		//{return m_pscd.GetCount();}

protected:
	CMap<T_PSCD_K,T_PSCD_K,T_PSCD_D,T_PSCD_D&>m_pscd;
};

#endif