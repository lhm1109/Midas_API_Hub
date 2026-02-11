#ifndef __DB_PZEF_DB_H__
#define __DB_PZEF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PZEF
{
public:
	CDB_PZEF();
	virtual ~CDB_PZEF();

public:
	void Add(T_PZEF_D& rData);
	BOOL Del();

public:
	BOOL Get(T_PZEF_D& rData)
		{return m_pzef.Lookup(1,rData);}
	int GetCount()
		{return m_pzef.GetCount();}

protected:
	CMap<T_PZEF_K,T_PZEF_K,T_PZEF_D,T_PZEF_D&>m_pzef;
};

#endif