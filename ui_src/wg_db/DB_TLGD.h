#ifndef __DB_TLGD_DB_H__
#define __DB_TLGD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TLGD
{
public:
	CDB_TLGD();
	virtual ~CDB_TLGD();
	
public:
	void Add(T_TLGD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_TLGD_D& rData)
		{return m_tlgd.Lookup(1,rData);}
	int GetCount()
		{return m_tlgd.GetCount();}

protected:
	CMap<T_TLGD_K,T_TLGD_K,T_TLGD_D,T_TLGD_D&>m_tlgd;
};

#endif