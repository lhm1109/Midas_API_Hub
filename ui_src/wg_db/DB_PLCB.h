#ifndef __DB_PLCB_DB_H__
#define __DB_PLCB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PLCB
{
public:
	CDB_PLCB();
	virtual ~CDB_PLCB();
	CDBDoc* m_pDoc;

public:
	void Add(T_PLCB_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PLCB_D& rData);
		//{return m_plcb.Lookup(1,rData);}
	int GetCount();
		//{return m_plcb.GetCount();}

protected:
	CMap<T_PLCB_K,T_PLCB_K,T_PLCB_D,T_PLCB_D&>m_plcb;
};

#endif