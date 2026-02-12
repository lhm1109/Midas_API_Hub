#ifndef __DB_GDLN_DB_H__
#define __DB_GDLN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GDLN
{
public:
	CDB_GDLN();
	virtual ~CDB_GDLN();

public:
	T_GDLN_K m_nStartNum;

public:
	void Add(T_GDLN_K Key,T_GDLN_D& rData);
	BOOL Del(T_GDLN_K Key);

public:
	BOOL Get(T_GDLN_K Key,T_GDLN_D& rData)
		{return m_gdln.Lookup(Key,rData);}
	int GetCount()
		{return m_gdln.GetCount();}
	POSITION GetStart()
		{return m_gdln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GDLN_K& rKey,T_GDLN_D& rData)
		{m_gdln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GDLN_K,T_GDLN_K,T_GDLN_D,T_GDLN_D&>m_gdln;
};

#endif