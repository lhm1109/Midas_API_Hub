#ifndef __DB_RCCG_DB_H__
#define __DB_RCCG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

//class CDBDoc;

class CDB_RCCG
{
public:
	CDB_RCCG();
	virtual ~CDB_RCCG();
	//CDBDoc* m_pDoc;

public:
	void Add(T_RCCG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_RCCG_D& rData);
		//{return m_rccg.Lookup(1,rData);}
	int GetCount();
		//{return m_rccg.GetCount();}

protected:
	CMap<T_RCCG_K,T_RCCG_K,T_RCCG_D,T_RCCG_D&>m_rccg;
};

#endif