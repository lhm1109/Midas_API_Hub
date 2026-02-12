#ifndef __DB_MVLDch_DB_H__
#define __DB_MVLDch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDch
{
public:
	CDB_MVLDch();
	virtual ~CDB_MVLDch();
	CDBDoc* m_pDoc;

public:
	T_MVLDch_K m_nStartNum;
	T_MVLDch_K m_nLastNum;

public:
	void Add(T_MVLDch_K Key,T_MVLDch_D& rData);
	BOOL Del(T_MVLDch_K Key);

public:
	BOOL Get(T_MVLDch_K Key,T_MVLDch_D& rData);
		//{return m_mvldch.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvldch.GetCount();}
	POSITION GetStart();
		//{return m_mvldch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLDch_K& rKey,T_MVLDch_D& rData);
		//{m_mvldch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLDch_K,T_MVLDch_K,T_MVLDch_D,T_MVLDch_D&>m_mvldch;
};

#endif
