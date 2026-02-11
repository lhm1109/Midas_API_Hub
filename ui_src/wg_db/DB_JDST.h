#ifndef __DB_JDST_DB_H__
#define __DB_JDST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDST
{
public:
	CDB_JDST();
	virtual ~CDB_JDST();

public:
	void Add(T_JDST_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDST_D& rData)
		{return m_JDST.Lookup(1,rData);}
	int GetCount()
		{return m_JDST.GetCount();}

protected:
	CMap<T_JDST_K,T_JDST_K,T_JDST_D,T_JDST_D&> m_JDST;
};

#endif