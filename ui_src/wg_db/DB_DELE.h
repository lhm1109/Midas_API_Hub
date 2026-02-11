#ifndef __DB_DELE_DB_H__
#define __DB_DELE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_DELE
{
public:
	CDB_DELE();
	virtual ~CDB_DELE();

public:
	void Add(T_DELE_D& rData,CDB_ELEM* pElem);
	BOOL Del(CDB_ELEM* pElem);

public:
	BOOL Get(T_DELE_D& rData)
		{return m_dele.Lookup(1,rData);}
	int GetCount()
		{return m_dele.GetCount();}

protected:
	CMap<T_DELE_K,T_DELE_K,T_DELE_D,T_DELE_D&>m_dele;
};

#endif