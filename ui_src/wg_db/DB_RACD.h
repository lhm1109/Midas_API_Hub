#ifndef __DB_RACD_DB_H__
#define __DB_RACD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RACD
{
public:
	CDB_RACD();
	virtual ~CDB_RACD();
	CDBDoc* m_pDoc;

public:
	void Add(T_RACD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RACD_D& rData);
		//{return m_racd.Lookup(1,rData);}
	int GetCount();
		//{return m_racd.GetCount();}

protected:
	CMap<T_RACD_K,T_RACD_K,T_RACD_D,T_RACD_D&>m_racd;
};

#endif