#ifndef __DB_MTBG_DB_H__
#define __DB_MTBG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MTBG
{
public:
	CDB_MTBG();
	virtual ~CDB_MTBG();
	
public:
	void Add(T_MTBG_D& rData);
	BOOL Del();

public:
	BOOL Get(T_MTBG_D& rData)
		{return m_mtbg.Lookup(1,rData);}
	int GetCount()
		{return m_mtbg.GetCount();}

protected:
	CMap<T_MTBG_K,T_MTBG_K,T_MTBG_D,T_MTBG_D&>m_mtbg;
};

#endif