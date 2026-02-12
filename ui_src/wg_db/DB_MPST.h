#ifndef __DB_MPST_DB_H__
#define __DB_MPST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_MPST
{
public:
	CDB_MPST();
	virtual ~CDB_MPST();

public:
	void Add(T_MPST_K Key,T_MPST_D& rData,CDB_SECT* pSect);
	BOOL Del(T_MPST_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_MPST_K Key,T_MPST_D& rData)
		{return m_mpst.Lookup(Key,rData);}
	int GetCount()
		{return m_mpst.GetCount();}
	POSITION GetStart()
		{return m_mpst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MPST_K& rKey,T_MPST_D& rData)
		{m_mpst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MPST_K,T_MPST_K,T_MPST_D,T_MPST_D&>m_mpst;
};

#endif