#ifndef __DB_IEHC_DB_H__
#define __DB_IEHC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IEHC
{
public:
	CDB_IEHC();
	virtual ~CDB_IEHC();

public:
	void Add(T_IEHC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_IEHC_D& rData)
		{return m_iehc.Lookup(1,rData);}
	int GetCount()
		{return m_iehc.GetCount();}

protected:
	CMap<T_IEHC_K,T_IEHC_K,T_IEHC_D,T_IEHC_D&>m_iehc;
};

#endif