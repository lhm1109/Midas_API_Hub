#ifndef __DB_DASC_DB_H__
#define __DB_DASC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DASC
{
public:
	CDB_DASC();
	virtual ~CDB_DASC();

public:
	void Add(T_DASC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DASC_D& rData)
		{return m_dasc.Lookup(1,rData);}
	int GetCount()
		{return m_dasc.GetCount();}

protected:
	CMap<T_DASC_K,T_DASC_K,T_DASC_D,T_DASC_D&>m_dasc;
};

#endif