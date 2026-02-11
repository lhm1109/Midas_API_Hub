#ifndef __DB_JDDS_DB_H__
#define __DB_JDDS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDDS
{
public:
	CDB_JDDS();
	virtual ~CDB_JDDS();

public:
	void Add(T_JDDS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDDS_D& rData)
		{return m_JDDS.Lookup(1,rData);}
	int GetCount()
		{return m_JDDS.GetCount();}

protected:
	CMap<T_JDDS_K,T_JDDS_K,T_JDDS_D,T_JDDS_D&> m_JDDS;
};

#endif