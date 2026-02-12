#ifndef __DB_JDMS_DB_H__
#define __DB_JDMS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDMS
{
public:
	CDB_JDMS();
	virtual ~CDB_JDMS();

public:
	void Add(T_JDMS_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDMS_D& rData)
		{return m_JDMS.Lookup(1,rData);}
	int GetCount()
		{return m_JDMS.GetCount();}

protected:
	CMap<T_JDMS_K,T_JDMS_K,T_JDMS_D,T_JDMS_D&> m_JDMS;
};

#endif