#ifndef __DB_NMRD_DB_H__
#define __DB_NMRD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NMRD
{
public:
	CDB_NMRD();
	virtual ~CDB_NMRD();
	
public:
	void Add(T_NMRD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_NMRD_D& rData)
		{return m_nmrd.Lookup(1,rData);}
	int GetCount()
		{return m_nmrd.GetCount();}

protected:
	CMap<T_NMRD_K,T_NMRD_K,T_NMRD_D,T_NMRD_D&>m_nmrd;
};

#endif