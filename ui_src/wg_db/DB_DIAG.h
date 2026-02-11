#ifndef __DB_DIAG_DB_H__
#define __DB_DIAG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DIAG
{
public:
	CDB_DIAG();
	virtual ~CDB_DIAG();

public:
	T_DIAG_K m_nStartNum;
	T_DIAG_K m_nLastNum;

public:
	void Add(T_DIAG_K Key,T_DIAG_D& rData);
	BOOL Del(T_DIAG_K Key);

public:
	BOOL Get(T_DIAG_K Key,T_DIAG_D& rData)
		{return m_diag.Lookup(Key,rData);}
	int GetCount()
		{return m_diag.GetCount();}
	POSITION GetStart()
		{return m_diag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DIAG_K& rKey,T_DIAG_D& rData)
		{m_diag.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DIAG_K,T_DIAG_K,T_DIAG_D,T_DIAG_D&>m_diag;
};

#endif