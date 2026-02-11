#ifndef __DB_LTOM_DB_H__
#define __DB_LTOM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LTOM
{
public:
	CDB_LTOM();
	virtual ~CDB_LTOM();

public:
	void Add(T_LTOM_D& rData);
	BOOL Del();

public:
	BOOL Get(T_LTOM_D& rData)
		{return m_ltom.Lookup(1,rData);}
	int GetCount()
		{return m_ltom.GetCount();}

protected:
	CMap<T_LTOM_K,T_LTOM_K,T_LTOM_D,T_LTOM_D&>m_ltom;
};

#endif