#ifndef __DB_DVMT_DB_H__
#define __DB_DVMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DVMT
{
public:
	CDB_DVMT();
	virtual ~CDB_DVMT();
	
public:
	void Add(T_DVMT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DVMT_D& rData)
		{return m_dvmt.Lookup(1,rData);}
	int GetCount()
		{return m_dvmt.GetCount();}

protected:
	CMap<T_DVMT_K,T_DVMT_K,T_DVMT_D,T_DVMT_D&>m_dvmt;
};

#endif