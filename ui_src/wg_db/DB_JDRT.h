#ifndef __DB_JDRT_DB_H__
#define __DB_JDRT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDRT
{
public:
	CDB_JDRT();
	virtual ~CDB_JDRT();

public:
	void Add(T_JDRT_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDRT_D& rData)
		{return m_JDRT.Lookup(1,rData);}
	int GetCount()
		{return m_JDRT.GetCount();}

protected:
	CMap<T_JDRT_K,T_JDRT_K,T_JDRT_D,T_JDRT_D&> m_JDRT;
};

#endif