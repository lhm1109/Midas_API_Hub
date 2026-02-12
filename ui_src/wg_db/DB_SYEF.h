#ifndef __DB_SYEF_DB_H__
#define __DB_SYEF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SYEF
{
public:
	CDB_SYEF();
	virtual ~CDB_SYEF();

public:
	T_SYEF_K m_nStartNum;
	T_SYEF_K m_nLastNum;

public:
	void Add(T_SYEF_K Key,T_SYEF_D& rData);
	BOOL Del(T_SYEF_K Key);

public:
	BOOL Get(T_SYEF_K Key,T_SYEF_D& rData)
		{return m_syef.Lookup(Key,rData);}
	int GetCount()
		{return m_syef.GetCount();}
	POSITION GetStart()
		{return m_syef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SYEF_K& rKey,T_SYEF_D& rData)
		{m_syef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SYEF_K,T_SYEF_K,T_SYEF_D,T_SYEF_D&>m_syef;
};

#endif