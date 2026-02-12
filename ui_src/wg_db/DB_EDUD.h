#ifndef __DB_EDUD_DB_H__
#define __DB_EDUD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN.h"

class CDB_EDUD
{
public:
	CDB_EDUD();
	virtual ~CDB_EDUD();

public:
	void Add(T_EDUD_D& rData);
	BOOL Del();

public:
	BOOL Get(T_EDUD_D& rData)
	{return m_EDUD.Lookup(1,rData);}
	int GetCount()
	{return m_EDUD.GetCount();}

protected:
	CMap<T_EDUD_K,T_EDUD_K,T_EDUD_D,T_EDUD_D&> m_EDUD;
};

#endif