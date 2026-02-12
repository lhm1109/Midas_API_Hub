#ifndef __DB_JDPL_DB_H__
#define __DB_JDPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDPL
{
public:
	CDB_JDPL();
	virtual ~CDB_JDPL();

public:
	void Add(T_JDPL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDPL_D& rData)
		{return m_JDPL.Lookup(1,rData);}
	int GetCount()
		{return m_JDPL.GetCount();}

protected:
	CMap<T_JDPL_K,T_JDPL_K,T_JDPL_D,T_JDPL_D&> m_JDPL;
};

#endif