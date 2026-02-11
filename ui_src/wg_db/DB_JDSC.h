#ifndef __DB_JDSC_DB_H__
#define __DB_JDSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDSC
{
public:
	CDB_JDSC();
	virtual ~CDB_JDSC();

public:
	void Add(T_JDSC_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDSC_D& rData)
		{return m_JDSC.Lookup(1,rData);}
	int GetCount()
		{return m_JDSC.GetCount();}

protected:
	CMap<T_JDSC_K,T_JDSC_K,T_JDSC_D,T_JDSC_D&> m_JDSC;
};

#endif