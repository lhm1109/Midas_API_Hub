#ifndef __DB_TDAT_DB_H__
#define __DB_TDAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDAT
{
public:
	CDB_TDAT();
	virtual ~CDB_TDAT();

public:
	T_TDAT_K m_nStartNum;
	T_TDAT_K m_nLastNum;

public:
	void Add(T_TDAT_K Key,T_TDAT_D& rData);
	BOOL Del(T_TDAT_K Key);

public:
	BOOL Get(T_TDAT_K Key,T_TDAT_D& rData)
		{return m_tdat.Lookup(Key,rData);}
	int GetCount()
		{return m_tdat.GetCount();}
	POSITION GetStart()
		{return m_tdat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDAT_K& rKey,T_TDAT_D& rData)
		{m_tdat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDAT_K,T_TDAT_K,T_TDAT_D,T_TDAT_D&>m_tdat;
};

#endif
