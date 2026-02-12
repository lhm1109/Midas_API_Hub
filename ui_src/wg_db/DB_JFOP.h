#ifndef __DB_JFOP_DB_H__
#define __DB_JFOP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JFD.h"

class CDB_JFOP
{
public:
	CDB_JFOP();
	virtual ~CDB_JFOP();

public:
	void Add(T_JFOP_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JFOP_D& rData)
		{return m_JFOP.Lookup(1,rData);}
	int GetCount()
		{return m_JFOP.GetCount();}

protected:
	CMap<T_JFOP_K,T_JFOP_K,T_JFOP_D,T_JFOP_D&> m_JFOP;
};

#endif