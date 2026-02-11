#ifndef __DB_EFWD_DB_H__
#define __DB_EFWD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_EFWD
{
public:
	CDB_EFWD();
	virtual ~CDB_EFWD();

public:
	void Add(T_EFWD_K Key,T_EFWD_D& rData,CDB_SECT* pSect);
	BOOL Del(T_EFWD_K Key,CDB_SECT* pSect);

public:
	// EFWD
	BOOL Get(T_EFWD_K Key,T_EFWD_D& rData)
	{return m_EFWD.Lookup(Key,rData);}
	int GetCount()
	{return m_EFWD.GetCount();}
	POSITION GetStart()
	{return m_EFWD.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EFWD_K& rKey,T_EFWD_D& rData)
	{m_EFWD.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EFWD_K,T_EFWD_K,T_EFWD_D,T_EFWD_D&>m_EFWD;
};

#endif