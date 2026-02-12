#ifndef __DB_JDEL_DB_H__
#define __DB_JDEL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDEL
{
public:
	CDB_JDEL();
	virtual ~CDB_JDEL();

public:
	void Add(T_JDEL_D& rData);
	BOOL Del();

public:
	BOOL Get(T_JDEL_D& rData)
		{return m_JDEL.Lookup(1,rData);}
	int GetCount()
		{return m_JDEL.GetCount();}

protected:
	CMap<T_JDEL_K,T_JDEL_K,T_JDEL_D,T_JDEL_D&> m_JDEL;
};

#endif