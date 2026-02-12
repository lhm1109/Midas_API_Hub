#ifndef __DB_JDCR_DB_H__
#define __DB_JDCR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDCR
{
public:
	CDB_JDCR();
	virtual ~CDB_JDCR();

public:
	void Add(T_JDCR_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDCR_D& rData)
		{return m_JDCR.Lookup(1,rData);}
	int GetCount()
		{return m_JDCR.GetCount();}

protected:
	CMap<T_JDCR_K,T_JDCR_K,T_JDCR_D,T_JDCR_D&> m_JDCR;
};

#endif